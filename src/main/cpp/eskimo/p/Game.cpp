// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Ken Leung. All rights reserved.

#include "core/XConfig.h"
#include "Box2D/Box2D.h"
#include "core/CCSX.h"
#include "s/GEngine.h"
#include "HUD.h"
#include "n/Platform.h"
#include "n/GSwitch.h"
#include "n/Igloo.h"
#include "n/Eskimo.h"
#include "Game.h"
#include "Splash.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(eskimo)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() {
    return (HUDLayer*)getSceneX()->getLayer(3); }

  DECL_PTR(a::NodeList, shared)

  STATIC_REIFY_LAYER(GLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(2)

  void onAcceleration(c::Acceleration*, c::Event*);
  void clearLayer();
  void resetLevel();
  void newLevel();
  void loadLevel(int);

  virtual bool onTouchStart(c::Touch*);
  virtual void onTouchEnd(c::Touch*);

  virtual void postReify();
  void createScreen(GVars*);
  void initPhysics(GVars*);
  int _tutorialStep;
  float _tutorialCounter ;

  int _currentLevel;
  int _gravity;


  c::Vec2 _acceleration;
  Igloo* _igloo;
  Eskimo* _player;
  b2World* _world;

  c::Sprite * _btnStart;
  c::Sprite * _btnReset;
  c::Sprite * _btnPause;
  c::Sprite * _btnAgain;
  c::Sprite * _btnMenu;
  c::Label * _messages;
  c::Label * _tutorialLabel;
    c::ValueVector _levels;

  c::ParticleSystem * _smoke;

  s_vec<c::Sprite*> _buttons;
};

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *touch) {
  auto tap = touch->getLocation();
  F__LOOP(it,_buttons) {
    auto btn = *it;
    if (btn->isVisible()) {
      if (btn->boundingBox().containsPoint(tap)) {
        CC_SHOW(btn->getChildByTag(kSpriteBtnOn));
        return true;
      }
    }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnd(c::Touch *touch) {
  auto tap = touch->getLocation();
  F__LOOP(it, _buttons) {
    auto btn= *it;
    if (!btn->isVisible()) {
    continue;
    }

    if (btn->boundingBox().containsPoint(tap)) {
      CC_HIDE(btn->getChildByTag(kSpriteBtnOn));
      switch (btn->getTag()) {
        case kSpriteBtnAgain:
          MGMS()->stop();
          CC_HIDE(_btnAgain);
          CC_HIDE(_btnMenu);
          resetLevel();
          cx::sfxPlay("button");
        break;
        case kSpriteBtnReset:
          MGMS()->stop();
          resetLevel();
        break;
        case kSpriteBtnPause:
          if (MGMS()->isLive()) {
            _messages->setString("paused");
            CC_SHOW(_messages);
          } else {
            CC_HIDE(_messages);
          }
          //_running = !_running;
        break;
        case kSpriteBtnStart:
          cx::sfxPlay("button");
          CC_SHOW(_btnPause);
          CC_SHOW(_btnReset);
          CC_HIDE(_btnStart);
          CC_HIDE(_messages);
          MGMS()->play();
          if (_currentLevel == 0) {
            CC_SHOW(_tutorialLabel);
          }
        break;
        case kSpriteBtnMenu:
          cx::sfxPlay("button");
          cx::pauseMusic();
          unscheduleUpdate();
          CC_DTOR()->replaceScene(
            c::TransitionMoveInL::create(0.2f, Splash::reify()));
        break;
      }
      return;
    }
  }

  if (!MGMS()->isLive()) { return; }
  SCAST(EskimoSprite*, _player->node)->_switchShape=true;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::initPhysics(GVars *ss) {
  _player = mc_new1(Eskimo,ss);
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::postReify() {
  shared = engine->getNodeList(SharedNode().typeId());
  auto ss= CC_GNLF(GVars,shared,"slots");

  _levels = c::FileUtils::getInstance()->getValueVectorFromFile("pics/levels.plist");

  createScreen(ss);
  initPhysics(ss);

  c::Device::setAccelerometerEnabled(true);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(
      c::EventListenerAcceleration::create(
        CC_CALLBACK_2(GLayer::onAcceleration, this)),
      this);

  auto ctx= (GameCtx*) MGMS()->getCtx();
  loadLevel(ctx->level);

  cx::sfxMusic("background", true);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::loadLevel(int level) {

  clearLayer();

  _currentLevel = level;
  resetLevel();


  //create platforms
  auto levelData = _levels.at(_currentLevel).asValueMap();
  auto platforms = levelData.at("platforms").asValueVector();
  auto po= MGMS()->getPool("Platforms");

  for (auto platformData : platforms) {
      auto pm = (f::ComObj*)po->getAndSet(true);
    auto data = platformData.asValueMap();
    SCAST(PlatformSprite*,pm->node)
    ->initPlatform (
                            data.at("width").asInt() * TILE,
                            data.at("angle").asFloat(),
                            c::Vec2(data.at("x").asFloat() * TILE,
                                 data.at("y").asFloat() * TILE));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::resetLevel() {
  /*
   level initialization is broken into loadLevel and resetLevel because there are
   actions that need to be taken only when loading the new level, and actions that
   need to be taken when loading a new level AND resetting the level (when user presses
   play again or the reset level button)
   */

  auto levelData = _levels.at(_currentLevel).asValueMap();

  CC_HIDE(_btnReset);
  CC_HIDE(_btnPause);

  //set up world gravity based on level data value
  _gravity = levelData.at("gravity").asInt();

  int i = 1;
  CC_DTOR()->getEventDispatcher()->dispatchCustomEvent(NOTIFICATION_GRAVITY_SWITCH, &i);

  switch (_gravity) {
    case kDirectionUp:
        _world->SetGravity(b2Vec2(0,FORCE_GRAVITY));
    break;
    case kDirectionDown:
        _world->SetGravity(b2Vec2(0,-FORCE_GRAVITY));
    break;
    case kDirectionLeft:
        _world->SetGravity(b2Vec2(-FORCE_GRAVITY, 0));
    break;
    case kDirectionRight:
        _world->SetGravity(b2Vec2(FORCE_GRAVITY, 0));
    break;
  }

  //create switches
  auto switches = levelData.at("switches").asValueVector();
  auto po= MGMS()->getPool("Switches");
  for (auto i = 0; i < switches.size();  ++i) {
    auto data = switches.at(i).asValueMap();
    auto gs = (GSwitch*)po->getAndSet(true);
    if ( i< switches.size()) {
      gs->initGSwitch(
                     data.at("gravity").asInt(),
                     c::Vec2(data.at("x").asFloat() * TILE,
                          data.at("y").asFloat() * TILE));
    } else {
        gs->hide();
    }
    //++i;
  }

  //reset player to level start position
  SCAST(EskimoSprite*,_player->node)->setSpritePosition(
      c::Vec2(levelData.at("startx").asFloat() * TILE,
              levelData.at("starty").asFloat() * TILE));
  _player->show();
  SCAST(EskimoSprite*,_player->node)->reset();

  //reset igloo to level end position
  _igloo->initIgloo(_gravity, c::Vec2(levelData.at("endx").asFloat() * TILE,
                                  levelData.at("endy").asFloat() * TILE));
  //reset smoke particle to match igloo position
  _smoke->setPosition(_igloo->getPositionX() + TILE,
                      _igloo->getPositionY() + TILE * 0.5f);
  //hide smoke. only shown when level is completed
  CC_HIDE(_smoke);
  _smoke->stopSystem();

  //if _currentLevel == 0 this is tutorial level
  if (_currentLevel != 0) {
    _messages->setString( "level "+ s::to_string(_currentLevel));
  } else {
    _messages->setString("tutorial");
    _tutorialStep = 0;
    _tutorialCounter = 0.0;
  }
  CC_HIDE(_tutorialLabel);

  CC_SHOW(_messages);
  CC_SHOW(_btnStart);
  CC_HIDE(_btnMenu);
  CC_HIDE(_btnAgain);

  _acceleration = c::Vec2(0,0);
}

//////////////////////////////////////////////////////////////////////////////
//
//hide elements from previous level
void GLayer::clearLayer() {
  auto p=MGMS()->getPool("Platforms");

  p->foreach([=](f::Poolable *p) {
    auto s= (b2Sprite*) SCAST(Platform*,p)->node;
    s->_body->SetActive(false);
    s->setVisible(false);
  });

  p= MGMS()->getPool("Switches");
  p->foreach([=](f::Poolable *p) {
    SCAST(GSwitch*,p)->node->setVisible(false);
  });

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onAcceleration(c::Acceleration *acc, c::Event *event) {
  _acceleration = c::Vec2(acc->x * ACCELEROMETER_MULTIPLIER,
                        acc->y * ACCELEROMETER_MULTIPLIER);
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decorate() {

  b2Vec2 gravity; gravity.Set(0.0f, -FORCE_GRAVITY);

  _world = new b2World(gravity);
  _world->SetAllowSleeping(true);
  _world->SetContinuousPhysics(true);

  engine = mc_new1(GEngine, _world);
  centerImage("game.bg");
  regoAtlas("game-pics", kMiddleground);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createScreen(GVars *ss) {
  auto wb= cx::visBox();
  auto wz=cx::visRect();

  _igloo = Igloo::create(ss);
  _igloo->setVisible(false);
  addAtlasItem("game-pics", _igloo, kForeground);

  c::Sprite *buttonOn;
  //create buttons

  _btnStart = cx::reifySprite("btn_start_off.png");
  _btnStart->setPosition(wb.cx, wb.top * 0.45f);
  CC_HIDE(_btnStart);
  addAtlasItem("game-pics", _btnStart, kForeground, kSpriteBtnStart);
  buttonOn = cx::reifySprite("btn_start_on.png");
  buttonOn->setAnchorPoint(cx::anchorBL());
  CC_HIDE(buttonOn);
  _btnStart->addChild(buttonOn, kForeground, kSpriteBtnOn);
  _buttons.push_back(_btnStart);

  _btnReset = cx::reifySprite("btn_reset_off.png");
  _btnReset->setAnchorPoint(cx::anchorT());
  _btnReset->setPosition(wb.right * 0.9f, wb.top);
  CC_HIDE(_btnReset);
  addAtlasItem("game-pics", _btnReset, kForeground, kSpriteBtnReset);
  buttonOn = cx::reifySprite("btn_reset_on.png");
  buttonOn->setAnchorPoint(cx::anchorBL());
  CC_HIDE(buttonOn);
  _btnReset->addChild(buttonOn, kForeground, kSpriteBtnOn);
  _buttons.push_back(_btnReset);

  _btnPause = cx::reifySprite("btn_pause_off.png");
  _btnPause->setAnchorPoint(cx::anchorT());
  _btnPause->setPosition(wb.right * 0.1f, wb.top);
  CC_HIDE(_btnPause);
  addAtlasItem("game-pics", _btnPause, kForeground, kSpriteBtnPause);
  buttonOn = cx::reifySprite("btn_pause_on.png");
  buttonOn->setAnchorPoint(cx::anchorBL());
  CC_HIDE(buttonOn);
  _btnPause->addChild(buttonOn, kForeground, kSpriteBtnOn);
  _buttons.push_back(_btnPause);

  _btnAgain = cx::reifySprite("btn_again_off.png");
  _btnAgain->setPosition(wb.cx, wb.top * 0.45f);
  CC_HIDE(_btnAgain);
  addAtlasItem("game-pics", _btnAgain, kForeground, kSpriteBtnAgain);
  buttonOn = cx::reifySprite("btn_again_on.png");
  buttonOn->setAnchorPoint(cx::anchorBL());
  CC_HIDE(buttonOn);
  _btnAgain->addChild(buttonOn, kForeground, kSpriteBtnOn);
  _buttons.push_back(_btnAgain);

  _btnMenu = cx::reifySprite("btn_menu_off.png");
  _btnMenu->setPosition(wb.cx, wb.top * 0.3f);
  CC_HIDE(_btnMenu);
  addAtlasItem("game-pics", _btnMenu, kForeground, kSpriteBtnMenu);
  buttonOn = cx::reifySprite("btn_menu_on.png");
  buttonOn->setAnchorPoint(cx::anchorBL());
  CC_HIDE(buttonOn);
  _btnMenu->addChild(buttonOn, kForeground, kSpriteBtnOn);
  _buttons.push_back(_btnMenu);

  _messages = cx::reifyBmfLabel("font_msgs", "level");
  _messages->setAlignment(c::TextHAlignment::CENTER);
  //_messages->setAnchorPoint(c::anchorC());
  CC_HIDE(_messages);
  _messages->setPosition(wb.cx, wb.top * 0.6f);
  addItem(_messages, kForeground);

  _smoke = c::ParticleSystemQuad::create("pics/smoke.plist");
  CC_HIDE(_smoke);
  _smoke->stopSystem();
  addItem(_smoke, kForeground);

  _tutorialLabel = cx::reifyLabel("dft", 25, TUTORIAL_1);
  _tutorialLabel->setAlignment(c::TextHAlignment::CENTER);
  _tutorialLabel->setPosition(wb.cx, wb.top * 0.2f);
  _tutorialLabel->setHeight(wz.size.height * 0.4f);
  _tutorialLabel->setWidth(wz.size.width * 0.7f);
  CC_HIDE(_tutorialLabel);
  addItem(_tutorialLabel, kForeground);

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::update(float dt) {

  auto py= CC_GNLF(Eskimo,players,"player");
  auto ss= CC_GNLF(GVars,shared,"slots");
  auto player= (EskimoSprite*) py->node;

  ss->world->Step(dt, 10, 10);
  ss->world->ClearForces();

  player->update();

    //if player is touching something, update it with accelerometer data
  if (player->_body->GetContactList()) {
    b2Vec2 vec;
    switch (ss->gravity) {
    //update X axis
      case kDirectionDown:
      case kDirectionUp:
        //if player is a circle, slow it down.
        if (player->_state == kStateCircle) {
          player->_body->ApplyForce(
              b2Vec2(ss->acceleration.x * CIRCLE_MULTIPLIER, 0),
              player->_body->GetWorldCenter(),
              true);

          vec = player->_body->GetLinearVelocity();
          vec.x *= BALL_FRICTION;
          player->_body->SetLinearVelocity(vec);
          //if player is a box don't apply any friction
        } else {
          player->_body->ApplyLinearImpulse(
              b2Vec2(ss->acceleration.x * BOX_MULTIPLIER, 0),
              player->_body->GetWorldCenter(),
              true);
        }
      break;

      //update Y axis
      case kDirectionRight:
      case kDirectionLeft:
        //if player is a circle, slow it down.
        if (player->_state == kStateCircle) {
          player->_body->ApplyForce(
              b2Vec2(0, ss->acceleration.y * CIRCLE_MULTIPLIER),
              player->_body->GetWorldCenter(),
              true);
          vec = player->_body->GetLinearVelocity();
          vec.y *= BALL_FRICTION;
          player->_body->SetLinearVelocity(vec);
          //if player is a box don't apply any friction
        } else {
          player->_body->ApplyLinearImpulse(
              b2Vec2(0, ss->acceleration.y * BOX_MULTIPLIER),
              player->_body->GetWorldCenter(),
              true);
        }
      break;
    }
  }

  auto &sws = MGMS()->getPool("Switches")->list();
  //track collision with GSwitches
  float diffx;
  float diffy;

  F__LOOP(it, sws) {
    auto gw= (*it)->node;
    if (!gw->isVisible())
    continue;
    diffx = gw->getPositionX() - player->getPositionX();
    diffy = gw->getPositionY() - player->getPositionY();
    if (pow(diffx, 2) + pow (diffy, 2) < PLAYER_SWITCH_RADII) {
      cx::sfxPlay("switch");
      ss->gravity = gw->direction;
      CC_HIDE(gw);
      //notify of collision

      EVENT_DISPATCHER->dispatchCustomEvent(NOTIFICATION_GRAVITY_SWITCH);
			//change world gravity
      switch (ss->gravity) {
        case kDirectionUp:
          ss->world->SetGravity(b2Vec2(0, FORCE_GRAVITY));
        break;
        case kDirectionDown:
          ss->world->SetGravity(b2Vec2(0, -FORCE_GRAVITY));
        break;
        case kDirectionLeft:
          ss->world->SetGravity(b2Vec2(-FORCE_GRAVITY, 0 ));
        break;
        case kDirectionRight:
          ss->world->SetGravity(b2Vec2(FORCE_GRAVITY, 0 ));
        break;
      }
      break;
    }
  }

  //check for level complete (collision with Igloo)
  diffx = player->getPositionX() - igloo->getPositionX();
  diffy = player->getPositionY() - igloo->getPositionY();
  if (pow(diffx, 2) + pow(diffy, 2) < IGLOO_SQ_RADIUS) {
    cx::sfxPlay("igloo");
    CC_HIDE(player);
    _btnPause->setVisible(false);
    _btnReset->setVisible(false);

    //notify of Level Completed. Igloo will listen to this and change its texture.
    EVENT_DISPATCHER->dispatchCustomEvent(NOTIFICATION_LEVEL_COMPLETED);

    //run smoke particle
    CC_SHOW(_smoke);
    _smoke->resetSystem();
    _messages->setString("well done!");
    CC_SHOW(_messages);

    //create delay until new level is loaded
    auto seq= c::Sequence::create(
        c::DelayTime::create(2.5f),
        c::CallFunc::create([=]() {
          this->newLevel();
          }),
        CC_NIL);
    this->runAction(seq);
    CC_HIDE(_tutorialLabel);
    MGMS()->stop();
  }

  //check for game over. player is off screen
  if (player->getPositionY() > wb.top ||
      player->getPositionY() < 0 ||
      player->getPositionX() > wb.right ||
      player->getPositionX() < 0) {

    cx::sfxPlay("oops");
    MGMS()->stop();
    CC_HIDE(player);
    CC_HIDE(_btnPause);
    CC_HIDE(_btnReset);

    _messages->setString("oops!");
    CC_SHOW(_messages);
    CC_SHOW(_btnAgain);
    CC_SHOW(_btnMenu);
  }

  //update text in tutorial level (currentLevel == 0)
  if (_currentLevel == 0) {
    _tutorialCounter += dt;
    if (_tutorialCounter > 15 && _tutorialStep == 1) {
      _tutorialStep = 2;
      _tutorialLabel->setString(TUTORIAL_3);
    } else if (_tutorialCounter > 5 && _tutorialStep == 0) {
      _tutorialStep = 1;
      _tutorialLabel->setString(TUTORIAL_2);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::newLevel() {

  if (_currentLevel + 1 >= _levels.size()) { return; }

  auto ctx= (GameCtx*) MGMS()->getCtx();

  loadLevel(_currentLevel + 1);

  if (_currentLevel > ctx->levelsCompleted) {
    ctx->levelsCompleted = _currentLevel;
    c::UserDefault::getInstance()->setIntegerForKey(
        "levelsCompleted",
        ctx->levelsCompleted);
    c::UserDefault::getInstance()->flush();
  }
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
  auto y= (GLayer*) getGLayer();
}

//////////////////////////////////////////////////////////////////////////////
void Game::decorate() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}



NS_END




