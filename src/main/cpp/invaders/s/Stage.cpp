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
void Stage::onAdd() {
  CannonCtrlNode c;
  ShipMotionNode s;

  cannonNode = engine->getNodeList(c.typeId());
  shipNode = engine->getNodeList(s.typeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Stage::update(float dt) {

  if (MGMS()->isLive()) {
    if (!inited) {
      onceOnly();
    }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Stage::onceOnly() {

  auto gun = CC_GNF(Cannon, cannonNode->head, "cannon");
  auto lpr= CC_GNF(Looper, cannonNode->head, "looper");
  auto ship= CC_GNF(Ship, shipNode->head, "ship");
  auto cfg= MGMS()->getLCfg()->getValue();

  // pre-population objects in pools
  engine->reifyExplosions();
  engine->reifyMissiles();
  engine->reifyBombs();

  lpr->timer7 = cx::reifyTimer(MGML(), JS_FLOAT(cfg["coolDownWindow"]));
  gun->hasAmmo=false;

  SENDMSGEX("/game/player/set!", ship);

  inited=true;
}


NS_END(invaders)

