// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

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

  __decl_md(c::ValueVector, _levels)
  __decl_ptr(b2World, _world)

  __decl_ptr(ecs::Node, _player)
  __decl_ptr(ecs::Node, _shared)

  __decl_create_layer(GLayer)
  __decl_deco_ui()
  __decl_get_iid(2)

  void onAcceleration(c::Acceleration*, c::Event*);
  void clearLayer();
  void resetLevel();
  void newLevel();
  void loadLevel(int);
  void readyNewLevel();

  virtual bool onMouseStart(const CCT_PT&);
  virtual void onMouseClick(const CCT_PT&);
  virtual bool onTouchStart(c::Touch*);
  virtual void onTouchEnd(c::Touch*);

  virtual void onInited();

  void createScreen(GVars*);
  void initPhysics(GVars*);
  void switchShape(bool);

  virtual ~GLayer() {
    mc_del_ptr(_world);
  }
};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::switchShape(bool t) {
  auto py= CC_GEC(Eskimo,_player,"f/CPixie");
  py->setSwitchShape( t);
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onMouseStart(const CCT_PT &tap) {
  return getHUD()->touchStart(tap);
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *touch) {
  return onMouseStart(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseClick(const CCT_PT &tap) {
  getHUD()->touchEnd(tap);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnd(c::Touch *touch) {
  onMouseClick(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::initPhysics(GVars *ss) {
  //_player = mc_new1(Eskimo,ss);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {
  _levels = CC_FILER()->getValueVectorFromFile("misc/levels.plist");
  _player = _engine->getNodes("f/CGesture")[0];
  _shared = _engine->getNodes("n/GVars")[0];

  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto ctx= (GameCtx*) MGMS()->getCtx();

  getHUD()->setG(ss);
  createScreen(ss);
  initPhysics(ss);

  c::Device::setAccelerometerEnabled(true);
  CC_ADD_ELX(this,
      c::EventListenerAcceleration::create(
        CC_CALLBACK_2(GLayer::onAcceleration, this)));

  loadLevel(ctx->level);
  cx::sfxMusic("background", true);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::loadLevel(int level) {

  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto po= MGMS()->getPool("Platforms");
  ss->currentLevel = level;

  clearLayer();
  resetLevel();

  //create platforms
  auto levelData = _levels.at(level).asValueMap();
  auto platforms =
    levelData.at("platforms").asValueVector();

  for (auto pData : platforms) {
    auto e= po->take(true);
    auto pm = CC_GEC(Platform,e,"f/CPixie");
    auto data = pData.asValueMap();
    pm->initPlatform (
                      data.at("width").asInt() * TILE,
                      data.at("angle").asFloat(),
                      CCT_PT(data.at("x").asFloat() * TILE,
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
  auto player= CC_GEC(Eskimo,_player,"f/CPixie");
  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto levelData =
    _levels.at(ss->currentLevel).asValueMap();
  float x=0;
  float y=0;

  getHUD()->toggleBtn(kSpriteBtnPause,false);
  getHUD()->toggleBtn(kSpriteBtnReset,false);

  //set up world gravity based on level data value
  ss->gravity = levelData.at("gravity").asInt();

  int i = 1;
  CC_DISP_CE2(NOTIFY_GSWITCH, &i);

  switch (ss->gravity) {
    case kDirectionUp: y= FORCE_GRAVITY; break;
    case kDirectionDown: y=  -FORCE_GRAVITY; break;
    case kDirectionLeft: x= -FORCE_GRAVITY; break;
    case kDirectionRight: x= FORCE_GRAVITY; break;
  }
  ss->world->SetGravity(b2Vec2(x,y));

  //create switches
  auto switches = levelData.at("switches").asValueVector();
  auto po= MGMS()->getPool("Switches");

  for (auto n = 0; n < switches.size();  ++n) {
    auto data = switches.at(n).asValueMap();
    auto e = po->take(true);
    auto gs = CC_GEC(GSwitch,e,"f/CPixie");
    if (n < switches.size()) {
      gs->initGSwitch(
                     data.at("gravity").asInt(),
                     CCT_PT(data.at("x").asFloat() * TILE,
                            data.at("y").asFloat() * TILE));
    } else {
      cx::hibernate((ecs::Node*)e);
    }
    //++n;
  }

  //reset player to level start position
  CC_POS2(player,
     levelData.at("startx").asFloat() * TILE,
      levelData.at("starty").asFloat() * TILE);
  CC_SHOW(player);
  player->reset();

  //reset igloo to level end position
  ss->igloo->initIgloo(ss->gravity,
      CCT_PT(levelData.at("endx").asFloat() * TILE,
             levelData.at("endy").asFloat() * TILE));

  //reset smoke particle to match igloo position
  CC_POS2(ss->smoke, ss->igloo->getPositionX() + TILE,
                     ss->igloo->getPositionY() + HTV(TILE));
  //hide smoke. only shown when level is completed
  CC_HIDE(ss->smoke);
  ss->smoke->stopSystem();

  //if _currentLevel == 0 this is tutorial level
  if (ss->currentLevel != 0) {
    getHUD()->showMsg("level " + FTOS(ss->currentLevel));
  } else {
    getHUD()->showMsg("tutorial");
    ss->tutorialStep = 0;
    ss->tutorialCounter = 0;
  }

  getHUD()->toggleTutorial(false);
  getHUD()->showMenu();

  ss->acc= CC_ZPT;
}

//////////////////////////////////////////////////////////////////////////////
//
//hide elements from previous level
void GLayer::clearLayer() {
  auto po=MGMS()->getPool("Platforms");

  po->foreach([=] (f::Poolable *p) {
    auto s=CC_GEC(Platform,p,"f/CPixie");
    s->getBody()->SetActive(false);
    cx::hibernate((ecs::Node*)p);
  });

  po= MGMS()->getPool("Switches");
  po->foreach([=](f::Poolable *p) {
    cx::hibernate((ecs::Node*)p);
  });

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onAcceleration(c::Acceleration *acc, c::Event *event) {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  ss->acc= CCT_PT(acc->x * ACCELEROMETER_MULTIPLIER,
                  acc->y * ACCELEROMETER_MULTIPLIER);
  //CCLOG("oh yeah! %f,%f", ss->acc.x, ss->acc.y);
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decoUI() {

  b2Vec2 gravity; gravity.Set(0, -FORCE_GRAVITY);

  _world = new b2World(gravity);
  _world->SetAllowSleeping(true);
  _world->SetContinuousPhysics(true);

  _engine = mc_new1(GEngine, _world);
  centerImage("game.bg");
  regoAtlas("game-pics", kMiddleground);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::createScreen(GVars *ss) {

  ss->smoke = c::ParticleSystemQuad::create(
      XCFG()->getAtlas("smoke"));
  ss->smoke->stopSystem();
  CC_HIDE(ss->smoke);
  addItem(ss->smoke, kForeground);

  ss->igloo = Igloo::create(ss);
  CC_HIDE(ss->igloo);
  addAtlasItem("game-pics", ss->igloo, kForeground);

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::newLevel() {
  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto ctx= (GameCtx*) MGMS()->getCtx();

  if (ss->currentLevel + 1 >= _levels.size()) {
  return; }

  loadLevel(ss->currentLevel + 1);

  if (ss->currentLevel > ctx->levelsCompleted) {
    ctx->levelsCompleted = ss->currentLevel;
    CC_APPDB()->setIntegerForKey("levelsCompleted", ctx->levelsCompleted);
    CC_APPDB()->flush();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::readyNewLevel() {
  auto seq= c::Sequence::create(
      c::DelayTime::create(2.5),
      c::CallFunc::create([=]() {
        this->newLevel();
        }),
      CC_NIL);
  this->runAction(seq);
  getHUD()->toggleTutorial(false);
  MGMS()->stop();
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
  auto y= (GLayer*) getGLayer();

  if (topic == "/game/player/switchshape->true") {
    y->switchShape(true);
  }
  else
  if (topic == "/game/player/resetlevel") {
    y->resetLevel();
  }
  else
  if (topic == "/game/player/newlevel") {
    y->newLevel();
  }
  else
  if (topic == "/game/hud/showmsg") {
    auto msg = (j::json*) m;
    y->getHUD()->showMsg(JS_STR(msg->operator[]("msg")));
  }
  else
  if (topic == "/game/unschedule") {
    y->unscheduleUpdate();
  }
  else
  if (topic == "/game/hud/togglebutton->off") {
    auto msg = (j::json*) m;
    y->getHUD()->toggleBtn(
        JS_INT(msg->operator[]("tag")), false);
  }
  else
  if (topic == "/game/hud/togglebutton->on") {
    auto msg = (j::json*) m;
    y->getHUD()->toggleBtn(
        JS_INT(msg->operator[]("tag")), true);
  }
  else
  if (topic == "/game/player/goto/newlevel") {
    y->readyNewLevel();
  }
  else
  if (topic == "/game/hud/showtut") {
    auto msg = (j::json*) m;
    y->getHUD()->showTut(JS_STR(msg->operator[]("msg")));
  }
}

//////////////////////////////////////////////////////////////////////////////
void Game::decoUI() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}



NS_END




