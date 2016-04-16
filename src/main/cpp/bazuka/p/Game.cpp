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
#include "ScrollingBgLayer.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(bazuka)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
struct CC_DLL GLayer : public f::GameLayer {

  HUDLayer* getHUD() {
    return (HUDLayer*)getSceneX()->getLayer(3); }

  void fireRocket(Hero*);
  void onStop();

  __decl_ptr(ScrollingBgLayer, _bgLayer)
  __decl_ptr(ecs::Node, _player)
  __decl_ptr(ecs::Node, _shared)

  __decl_create_layer(GLayer)
  __decl_deco_ui()
  __decl_get_iid(2)

  virtual void onMouseMotion(const CCT_PT&);
  virtual bool onMouseStart(const CCT_PT&);
  virtual void onMouseClick(const CCT_PT&);

  virtual void onTouchMotion(c::Touch*);
  virtual bool onTouchStart(c::Touch*);
  virtual void onTouchEnd(c::Touch*);
  virtual void onInited();

  virtual ~GLayer() {}

};

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onStop() {
  MGMS()->stop();
  surcease();
  Ende::reify(MGMS(), 4);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onInited() {
  _player = _engine->getNodes("f/CGesture")[0];
  _shared = _engine->getNodes("n/GVars")[0];

  auto hero=CC_GEC(Hero,_player,"f/CPixie");
  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto wz= cx::visSize();
  auto wb= cx::visBox();

  // player setup
  hero->inflate(wb.right * 0.125, HTV(wb.top));
  addItem(hero, 5);

  // player idle
  auto anim = cx::createAnimation(0.25,false,0);
  for (auto n=1; n <= 4; ++n) {
    auto s= cx::getSpriteFrame("player_idle_"+FTOS(n)+".png");
        anim->addSpriteFrame(s);
  }
  hero->idle= c::RepeatForever::create(c::Animate::create(anim));
  CC_KEEP(hero->idle);

  // player boost
  anim = cx::createAnimation(0.25, false, 0);
  for (auto n=1; n <= 4; ++n) {
    auto s= cx::getSpriteFrame("player_boost_"+FTOS(n)+".png");
    anim->addSpriteFrame(s);
  }
  hero->boost= c::RepeatForever::create(c::Animate::create(anim));
  CC_KEEP(hero->boost);

  hero->runAction(hero->boost->clone());
  ss->gravity = CCT_PT(0, -5);
  ss->bgLayer=this->_bgLayer;
}

//////////////////////////////////////////////////////////////////////////////
void GLayer::decoUI() {

  auto roll= CC_CSV(c::Float, "BG+SCROLL");
  auto wz= cx::visSize();
  auto wb= cx::visBox();

  _bgLayer = f::reifyRefType<ScrollingBgLayer>();
  _bgLayer->set(roll);
  addItem(_bgLayer);

  regoAtlas("game-pics");
  _engine = mc_new(GEngine);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseClick(const CCT_PT &loc) {

  auto hero= CC_GEC(Hero,_player,"f/CPixie");
  auto wb= cx::visBox();

  if (MGMS()->isLive()) {
    if (loc.x < wb.cx) {
      // treat as left button pressed
      hero->jump=true;
    } else {
      fireRocket(hero);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onMouseMotion(const CCT_PT &loc) {
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onTouchStart(c::Touch *touch) {
  return onMouseStart( touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
bool GLayer::onMouseStart(const CCT_PT &loc) {
  auto hero= CC_GEC(Hero,_player,"f/CPixie");
  auto wb= cx::visBox();

  if (MGMS()->isLive()) {
    if (loc.x < wb.cx) {
      // treat as left button pressed
      hero->jump=true;
    } else {
      fireRocket(hero);
    }
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::fireRocket(Hero *hero) {

  auto po= MGMS()->getPool("Rockets");
  auto sz= hero->csize();
  auto pos = hero->pos();
  auto e= (ecs::Node*)po->take(true);
  cx::resurrect(e, pos.x + HWZ(sz), pos.y - CC_ZH(sz) * 0.05);
  cx::sfxPlay("fireRocket");

  auto emitter = c::ParticleExplosion::create();
  emitter->setPosition(c::ccpAdd(pos, CCT_PT(HWZ(sz) ,0 )));
  emitter->setStartColor(c::ccc4f(1.0, 1.0, 1.0, 1.0));
  emitter->setEndColor(c::ccc4f(0.0, 0.0, 0.0, 0.0));
  emitter->setTotalParticles(10);
  emitter->setLife(0.25);
  emitter->setSpeed(2.0);
  emitter->setSpeedVar(50.0);
  emitter->setAutoRemoveOnFinish(true);
  addItem(emitter, 0);
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchMotion(c::Touch *touch) {
}

//////////////////////////////////////////////////////////////////////////////
//
void GLayer::onTouchEnd(c::Touch *touch) {
  onMouseClick(touch->getLocation());
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
void Game::sendMsgEx(const MsgTopic &topic, void *m) {
  auto y= (GLayer*) getGLayer();

  if (topic == "/game/player/lose") {
    y->onStop();
  }
}

//////////////////////////////////////////////////////////////////////////////
void Game::decoUI() {
  HUDLayer::reify(this, 3);
  GLayer::reify(this, 2);
  play();
}

NS_END




