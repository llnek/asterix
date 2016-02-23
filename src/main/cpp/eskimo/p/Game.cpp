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
#include "core/CCSX.h"
#include "s/GEngine.h"
#include "HUD.h"
#include "Igloo.h"
#include "Game.h"
#include "Box2D/Box2D.h"

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
  void loadLevel(int);

  virtual void postReify();
  void createScreen(GVars*);
  void initPhysics(GVars*);
  int _tutorialStep;
  float _tutorialCounter ;

  int _currentLevel;
  int _gravity;
  int _currentLevel;
  int _platformPoolIndex;
  c::Vec2 _acceleration;
  Igloo* _igloo;
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
void GLayer::initPhysics(GVars *ss) {
  b2Vec2 gravity;
  gravity.Set(0.0f, -FORCE_GRAVITY);

  ss->world =
    _world = new b2World(gravity);

  _world->SetAllowSleeping(true);
  _world->SetContinuousPhysics(true);

  //_player = Eskimo::create(this);
  //_gameBatchNode->addChild(_player, kMiddleground);
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

  for (auto platformData : platforms) {
    auto platform = _platformPool.at(_platformPoolIndex);
    ++_platformPoolIndex;
    if (_platformPoolIndex == _platformPool.size()) _platformPoolIndex = 0;

    auto data = platformData.asValueMap();
    platform->initPlatform (
                            data.at("width").asInt() * TILE,
                            data.at("angle").asFloat(),
                            c::Vec2(data.at("x").asFloat() * TILE,
                                 data.at("y").asFloat() * TILE)
                            );
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

  for (auto i = 0; i < switches.size();  ++i) {
    auto data = switches.at(i).asValueMap();
    auto gswitch = _gSwitchPool.at(i);
    if ( i< switches.size()) {
      gswitch->initGSwitch(
                           data.at("gravity").asInt(),
                           c::Vec2(data.at("x").asFloat() * TILE,
                                data.at("y").asFloat() * TILE
                                ));
    } else {
      CC_HIDE(gswitch);
    }
    //++i;
  }

  //reset player to level start position
  _player->setSpritePosition(c::Vec2(levelData.at("startx").asFloat() * TILE,
                                 levelData.at("starty").asFloat() * TILE));
  CC_SHOW(_player);
  _player->reset();

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
  for (auto sprite : _platformPool) {
    sprite->getBody()->SetActive(false);
    sprite->setVisible(false);
  }
  for (auto sprite : _gSwitchPool) {
    sprite->setVisible(false);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onAcceleration(c::Acceleration *acc, c::Event *event) {
  _acceleration = c::Vec2(acc->x * ACCELEROMETER_MULTIPLIER,
                        acc->y * ACCELEROMETER_MULTIPLIER);
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decorate() {
  engine = mc_new(GEngine);
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




