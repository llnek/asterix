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

#include "Terrain.h"
#include "Player.h"
#include "C.h"

NS_BEGIN(victorian)

//////////////////////////////////////////////////////////////////////////////
//
void resetTerrain(not_null<ecs::Node*> node) {
  auto t=CC_GEC(Terrain,node,"f/CPixie");
  t->reset();
}

//////////////////////////////////////////////////////////////////////////////
//
void resetPlayer(not_null<ecs::Node*> node) {
  auto pm= CC_GEC(PlayerMotion,node,"f/CMove");
  auto ps= CC_GEC(PlayerStats,node,"f/CStats");
  auto py= CC_GEC(Player,node,"f/CPixie");
  auto wb=cx::visBox();

  pm->maxSpeed.x = PLAYER_INITIAL_SPEED;
  pm->speed.x = PLAYER_INITIAL_SPEED;
  pm->vel = CC_ZPT;
  pm->setFloating(py, false);
  py->setRotation(0);
  pm->nextPos.y = wb.top * 0.6;
  py->setPosition(wb.right * 0.2, pm->nextPos.y);
  ps->state = kPlayerMoving;
  pm->setJumping(false);
  pm->setHasFloated(false);
}

NS_END


