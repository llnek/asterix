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
#include "core/CCSX.h"
#include "s/GEngine.h"
#include "HUD.h"
#include "MMenu.h"
#include "Ende.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(bunny)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() {
    return (HUDLayer*)getSceneX()->getLayer(3); }

  void movePlayerIfPossible(const CCT_PT &tap);
  void movePlayerByTouch(const CCT_PT &tap);
  void explodeBombs(const CCT_PT &tap);

  bool onContactBegin(c::PhysicsContact&);
  void setPhysicsWorld(c::PhysicsWorld*);
  void onEnd();
  void onPause();

  __decl_ptr(c::PhysicsWorld, _pWorld);
  __decl_ptr(ecs::Node, _player)
  __decl_ptr(ecs::Node, _shared)

  __decl_create_layer(GLayer)
  __decl_deco_ui()
  __decl_get_iid(2)

  virtual void onAcceleration(c::Acceleration *acc, c::Event*);
  virtual void onMouseMotion(const CCT_PT&);
  virtual bool onMouseStart(const CCT_PT&);

  virtual void onTouchMotion(c::Touch*);
  virtual bool onTouchStart(c::Touch*);
  virtual void onInited();

  virtual ~GLayer() {}
};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {

  _player= _engine->getNodes("f/CGesture")[0];
  _shared= _engine->getNodes("n/GVars")[0];

  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto wz= cx::visSize();
  auto wb= cx::visBox();

  setPhysicsWorld(MGMS()->getPhysicsWorld());

  auto ln = c::EventListenerPhysicsContact::create();
  ln->onContactBegin = CC_CALLBACK_1(GLayer::onContactBegin, this);
  getEventDispatcher()->addEventListenerWithSceneGraphPriority(ln, this);

}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::setPhysicsWorld(c::PhysicsWorld *world) {
  //enable debug draw
#if COCOS2D_DEBUG
  world->setDebugDrawMask(c::PhysicsWorld::DEBUGDRAW_ALL);
#endif
  _pWorld = world;
  _pWorld->setGravity(CCT_PT(0, 0));
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onContactBegin(c::PhysicsContact &ct) {
  auto sp= CC_GEC(f::CPixie,_player,"f/CPixie");
  auto playerShape = sp->getPhysicsBody()->getShapes().front();
  if (playerShape != ct.getShapeA() &&
      playerShape != ct.getShapeB()) {
  } else {
    onEnd();
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onEnd() {

  auto po= MGMS()->getPool("Bombs");
  po->foreach([=](f::Poolable *p) {
      auto sp=CC_GEC(f::CPixie,p,"f/CPixie");
      sp->getPhysicsBody()->setEnabled(false);
      cx::hibernate((ecs::Node*)p);
  });

  cx::sfxPlay("uh");
  MGMS()->stop();
  surcease();
  Ende::reify(MGMS(), 4);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::movePlayerIfPossible(const CCT_PT &tap) {
  auto sp=CC_GEC(f::CPixie,_player,"f/CPixie");
  auto w2 = HWZ(CC_CSIZE(sp));
  auto wb= cx::visBox();

  if (sp->getBoundingBox().containsPoint(tap)) {

    if (tap.x >= w2 &&
        tap.x < wb.right - w2) {
      sp->setPositionX(tap.x);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::movePlayerByTouch(const CCT_PT &tap) {
  movePlayerIfPossible(tap);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::explodeBombs(const CCT_PT &tap) {
  auto po= MGMS()->getPool("Bombs");
  po->foreach([=](f::Poolable *p) {
    if (!p->status()) { return; }
    auto s=CC_GEC(f::CPixie,p,"f/CPixie");
    if(s->getBoundingBox().containsPoint(tap)){
      cx::sfxPlay("bomb");
      auto exp= c::ParticleExplosion::create();
      //auto explosion = ParticleSystemQuad::create("explosion.plist");
      exp->setPosition(s->getPosition());
      this->addItem(exp);
      /*explosion->setTotalParticles(800);
      explosion->setSpeed(3.5f);
      explosion->setLife(300.0f);  */
      s->getPhysicsBody()->setEnabled(false);
      cx::hibernate((ecs::Node*)p);
    }

  });

}


//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMotion(const CCT_PT &loc) {
  movePlayerByTouch(loc);
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onMouseStart(const CCT_PT &tap) {
  explodeBombs(tap);
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *touch) {
  return onMouseStart(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchMotion(c::Touch *touch) {
  movePlayerByTouch(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decoUI() {

  auto btn = cx::reifyMenuBtn("pause.png", "pause_pressed.png");
  auto sz= CC_CSIZE(btn);
  auto wz= cx::visSize();
  auto wb= cx::visBox();

  centerImage("game.bg");
  regoAtlas("game-pics");

  btn->setCallback([=](c::Ref*) { this->onPause(); });
  CC_POS2(btn, wb.right - HWZ(sz), HHZ(sz));
  addItem(cx::mkMenu(btn), 1);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
  setKeepScreenOnJni(true);
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
  JniBridge::showToast("Hello Java");
#endif

  setAccelerometerEnabled(true);
  _engine = mc_new(GEngine);
  //cx::sfxMusic("background", true);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onPause() {
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onAcceleration(c::Acceleration *acc, c::Event*) {
  auto sp=CC_GEC(f::CPixie,_player,"f/CPixie");
  movePlayerIfPossible(
      CCT_PT(sp->getPositionX() + (acc->x * 10),
             sp->getPositionY()));
}


END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
  auto y= (GLayer*) getGLayer();

  if (topic=="/game/hud/updatescore") {
    y->getHUD()->updateScore(10);
  }


}

//////////////////////////////////////////////////////////////////////////////
void Game::decoUI() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}

//////////////////////////////////////////////////////////////////////////////
//
Game::Game()
  : f::GameScene(true) {
}

NS_END




