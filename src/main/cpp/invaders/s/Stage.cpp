// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2015, Ken Leung. All rights reserved.

#include "x2d/GameScene.h"
#include "core/CCSX.h"
#include "n/GNodes.h"
#include "Stage.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
Stage::Stage(not_null<EFactory*> f, not_null<c::Dictionary*> d)

  : XSystem<EFactory>(f, d) {

}

//////////////////////////////////////////////////////////////////////////
//
void Stage::addToEngine(not_null<a::Engine*> e) {
  CannonCtrlNode c;
  cannons = e->getNodeList(c.typeId());
}

//////////////////////////////////////////////////////////////////////////
//
void Stage::initAlienSize() {
  auto z= cx::calcSize("purple_bug_0.png");
  //pick purple since it is the largest
  state->setObject(f::Size2::create(z.width,z.height), "alienSize");
}

//////////////////////////////////////////////////////////////////////////
//
void Stage::initShipSize() {
  auto z= cx::calcSize("ship_0.png");
  state->setObject(f::Size2::create(z.width, z.height), "shipSize");
}

//////////////////////////////////////////////////////////////////////////
//
bool Stage::update(float dt) {
  auto n= cannons->head;
  if (MGMS()->isLive()) {
    if (!inited) {
      onceOnly(n);
    }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Stage::onceOnly(a::Node *node) {

  factory->reifyExplosions();
  factory->reifyMissiles();
  factory->reifyBombs();

  auto gun = CC_GNF(Cannon, node, "cannon");
  auto lpr= CC_GNF(Looper, node, "looper");
  //auto ship= CC_GNF(Ship, node, "ship");
  auto cfg= MGMS()->getLCfg()->getValue();

  lpr->timer7 = cx::reifyTimer( MGML(), JS_FLOAT(cfg["coolDownWindow"]));
  gun->hasAmmo=false;
  inited=true;
}

NS_END(invaders)

