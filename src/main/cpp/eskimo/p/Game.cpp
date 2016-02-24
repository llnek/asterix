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

  DECL_PTR(a::NodeList, players)
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

  DECL_TD(c::ValueVector, _levels)
  DECL_PTR(b2World, _world)
  DECL_FZ(_tutorialCounter)
  DECL_IZ(_tutorialStep)

};

//////////////////////////////////////////////////////////////////////////////
//
GLayer::~GLayer() {
  mc_del_ptr(_world);
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *touch) {
  return getHUD()->touchStart(touch);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnd(c::Touch *touch) {
  getHUD()->touchEnd(touch);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::initPhysics(GVars *ss) {
  //_player = mc_new1(Eskimo,ss);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::postReify() {
  _levels = CC_FILER()->getValueVectorFromFile("pics/levels.plist");
  players = engine->getNodeList(EskimoNode().typeId());
  shared = engine->getNodeList(SharedNode().typeId());
  auto ss= CC_GNLF(GVars,shared,"slots");
  auto ctx= (GameCtx*) MGMS()->getCtx();

  createScreen(ss);
  initPhysics(ss);

  c::Device::setAccelerometerEnabled(true);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(
      c::EventListenerAcceleration::create(
        CC_CALLBACK_2(GLayer::onAcceleration, this)),
      this);

  loadLevel(ctx->level);
  cx::sfxMusic("background", true);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::loadLevel(int level) {

  auto ss=CC_GNLF(GVars,shared,"slots");
  auto po= MGMS()->getPool("Platforms");
  ss->currentLevel = level;

  clearLayer();
  resetLevel();

  //create platforms
  auto levelData = _levels.at(level).asValueMap();
  auto platforms = levelData.at("platforms").asValueVector();

  for (auto platformData : platforms) {
    auto pm = (Platform*)po->getAndSet(true);
    auto data = platformData.asValueMap();
    auto ps= (PlatformSprite*) pm->node;

    ps->initPlatform (
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
  auto py= CC_GNLF(Eskimo,players,"player");
  auto ss= CC_GNLF(GVars,shared,"slots");
  auto player= (EskimoSprite*) py->node;
  float x=0;
  float y=0;

  getHUD()->toggleBtn(kSpriteBtnPause,false);
  getHUD()->toggleBtn(kSpriteBtnReset,false);

  //set up world gravity based on level data value
  ss->gravity = levelData.at("gravity").asInt();

  int i = 1;
  CC_DISPEVENT(NOTIFY_GSWITCH, &i);

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
    auto gs = (GSwitch*)po->getAndSet(true);
    if ( n < switches.size()) {
      gs->initGSwitch(
                     data.at("gravity").asInt(),
                     c::Vec2(data.at("x").asFloat() * TILE,
                          data.at("y").asFloat() * TILE));
    } else {
      gs->deflate();
    }
    //++n;
  }

  //reset player to level start position
  player->setSpritePosition(
      c::Vec2(levelData.at("startx").asFloat() * TILE,
              levelData.at("starty").asFloat() * TILE));
  CC_SHOW(player);
  player->reset();

  //reset igloo to level end position
  ss->igloo->initIgloo(ss->gravity,
      c::Vec2(levelData.at("endx").asFloat() * TILE,
              levelData.at("endy").asFloat() * TILE));

  //reset smoke particle to match igloo position
  ss->smoke->setPosition(ss->igloo->getPositionX() + TILE,
                         ss->igloo->getPositionY() + TILE * 0.5f);
  //hide smoke. only shown when level is completed
  CC_HIDE(ss->smoke);
  ss->smoke->stopSystem();

  //if _currentLevel == 0 this is tutorial level
  if (ss->currentLevel != 0) {
    getHUD()->showMsg("level "+ s::to_string(ss->currentLevel));
  } else {
    getHUD()->showMsg("tutorial");
    ss->tutorialStep = 0;
    ss->tutorialCounter = 0.0;
  }
  getHUD()->hideTuturial();
  //CC_HIDE(_tutorialLabel);

  getHUD()->showMenu();
  //CC_SHOW(_messages);
  //CC_SHOW(_btnStart);
  //CC_HIDE(_btnMenu);
  //CC_HIDE(_btnAgain);

  ss->acc= c::Vec2(0,0);
}

//////////////////////////////////////////////////////////////////////////////
//
//hide elements from previous level
void GLayer::clearLayer() {
  auto p=MGMS()->getPool("Platforms");

  p->foreach([=](f::Poolable *p) {
    auto s= (b2Sprite*) SCAST(Platform*,p)->node;
    s->_body->SetActive(false);
    p->deflate();
  });

  p= MGMS()->getPool("Switches");
  p->reset();

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onAcceleration(c::Acceleration *acc, c::Event *event) {
  auto ss=CC_GNLF(GVars,shared,"slots");
  ss->acc= c::Vec2(acc->x * ACCELEROMETER_MULTIPLIER,
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

  ss->igloo = Igloo::create(ss);
  CC_HIDE(ss->igloo);
  addAtlasItem("game-pics", ss->igloo, kForeground);

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::newLevel() {
  auto ss= CC_GNLF(GVars,shared,"slots");
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

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
  auto y= (GLayer*) getGLayer();

  if (topic == "/game/player/newlevel") {

  }
  else
  if (topic == "/game/hud/showmsg") {

  }
  else {

  }

}

//////////////////////////////////////////////////////////////////////////////
void Game::decorate() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}



NS_END




