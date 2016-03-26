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

//////////////////////////////////////////////////////////////////////////////

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Move.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(mock)


//////////////////////////////////////////////////////////////////////////////
//
void Move::preamble() {
  _shared= _engine->getNodes("n/GVars")[0];
}

//////////////////////////////////////////////////////////////////////////////
//
bool Move::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::process(float dt) {

  auto pm=CC_GEC(PlayerMotion,_player,"f/CMove");
  auto py=CC_GEC(Player,_player,"f/CPixie");
  auto ps=CC_GEC(PlayerStats,_player,"f/CStats");
  auto te=CC_GEC(Terrain,_terrain,"f/CPixie");
  auto batch=MGML()->getAtlas("blank");

  if (py->getPositionY() < -py->height() ||
      py->getPositionX() < -HTV(py->width()) ) {
     SENDMSG("/game/stop");
     return;
  }

  py->update(dt);
  te->move(pm->vel.x);

  if (ps->state != kPlayerDying) { te->checkCollision(_player); }
  py->place();
  batch->setPositionY (0);
  if (pm->nextPos.y > wb.top * 0.6) {
    batch->setPositionY( (wb.top * 0.6 - pm->nextPos.y) * 0.8);
  }

  if (te->startTerrain && pm->vel.x > 0) {
    ss->speedIncreaseTimer += dt;
    if (ss->speedIncreaseTimer > ss->speedIncreaseInterval) {
      ss->speedIncreaseTimer = 0;
      pm->maxSpeed.x= pm->maxSpeed.x + 4;
      pm->maxSpeed.y= pm->maxSpeed.y + 4;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::onKeys(float dt) {
}


NS_END




