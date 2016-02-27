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
#include "MMenu.h"
#include "Ende.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(bazuka)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() { return (HUDLayer*)getSceneX()->getLayer(3); }


  DECL_PTR(a::NodeList, players)
  DECL_PTR(a::NodeList, shared)
  STATIC_REIFY_LAYER(GLayer)

  MDECL_DECORATE()
  MDECL_GET_IID(2)

  virtual bool onTouchStart(c::Touch*);
  virtual void onInited();

  virtual ~GLayer();
};

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *touch) {

  if (! MGMS()->isLive()) {
    return false;
  }

  auto ss=CC_GNLF(GVars, shared, "slots");
  auto loc= touch->getLocation();

  if (leftButton.containsPoint(loc)) {
    ss->jump = true;
  }

  if (rightButton.containsPoint(loc)) {
    fireRocket();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::fireRocket() {

  auto py=CC_GNLF(Hero,players,"hero");
  auto po=MGMS()->getPool("Rockets");
  auto sz= py->csize();
  auto pos= py->pos();
  auto r= po->getAndSet(true);

  cx::sfxPlay("fireRocket");

  r->inflate(pos.x + HWZ(sz), pos.y - sz.height * 0.05f);

  auto em= c::ParticleExplosion::create();
  em->setPosition(c::ccpAdd(pos, c::Vec2(HWZ(sz) ,0 )));
  em->setStartColor(c::ccc4f(1.0, 1.0, 1.0, 1.0));
  em->setEndColor(c::ccc4f(0.0, 0.0, 0.0, 0.0));
  em->setTotalParticles(10);
  em->setLife(0.25f);
  em->setSpeed(2.0f);
  em->setSpeedVar(50.0f);
  em->setAutoRemoveOnFinish(true);
  addItem(em, 0);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onStop() {
  surcease();
  Ende::reify(this, 4);
}

//////////////////////////////////////////////////////////////////////////////
//
GLayer::~GLayer() {
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {
  players = engine->getNodeList(PlayerNode().typeId());
  shared = engine->getNodeList(SharedNode().typeId());
  auto ss= CC_GNLF(GVars, shared, "slots");
  auto py= CC_GNLF(Hero,players,"hero");
  auto wz= cx::visRect();
  auto wb= cx::visBox();

  ss->gravity= c::Vec2(0, -5);
  ss->bgLayer= this->bgLayer;
  ss->jump=false;
  ss->jumpTimer=0;

  //idle animation
  auto idleanimation = c::Animation::create();
  for (auto i = 1; i <= 4; ++i) {
    idleanimation->addSpriteFrame(
      cx::getSpriteFrame("player_idle_" + s::to_string(i) + ".png"));
  }
  idleanimation->setDelayPerUnit(0.25f);
  mIdleAction = c::RepeatForever::create(c::Animate::create(idleanimation));
  mIdleAction->retain();

  //boost animation
  auto boostanimation = c::Animation::create();
  for (auto i = 1; i <= 4;  ++i) {
    boostanimation->addSpriteFrame(
      cx::getSpriteFrame("player_boost_" + s::to_string(i) + ".png"));
  }
  boostanimation->setDelayPerUnit(0.25f);

  mBoostAction = c::RepeatForever::create(c::Animate::create(boostanimation)) ;
  py->node->runAction(mBoostAction->clone());
  mBoostAction->retain();

  rightButton = c::Rect(HWZ(wz.size), 0, HWZ(wz.size), wz.size.height);
  leftButton = c::Rect(0, 0, HWZ(wz.size), wz.size.height);

  projectileLayer = new ProjectileLayer();
  addItem(projectileLayer);

  this->schedule(schedule_selector(GLayer::spawnEnemy),3.0);

}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decoUI() {

  auto bg = f::reifyRefType<ScrollingBgLayer>();
  addItem(bg,-1);

  regoAtlas("game-pics");
  engine = mc_new(GEngine);
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
  auto y= (GLayer*) getGLayer();
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




