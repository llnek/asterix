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

//////////////////////////////////////////////////////////////////////////////

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Move.h"
#include "n/Terrain.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(mock)


//////////////////////////////////////////////////////////////////////////////
//
void Move::preamble() {
  _terrain= _engine->getNodes("f/CAutoma")[0];
  _player= _engine->getNodes("f/CGesture")[0];
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
  auto wz= cx::visSize();
  auto wb= cx::visBox();
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
    //auto pos= py->pos();
  if (py->getPositionY() < - cx::getHeight(py) ||
      py->getPositionX() < - HTV(cx::getWidth(py))) {
    SENDMSG("/game/stop");
    return;
  }

  processPlayer(dt);
  te->move(pm->vel.x);

  if (ps->state != kPlayerDying) {
    te->checkCollision(_player);
  }

  //place player
  py->setPositionY(pm->nextPos.y);
  if (pm->vel.x > 0 &&
      py->getPositionX() < wb.right * 0.2) {
    py->setPositionX(py->getPositionX() + pm->vel.x);
    if (py->getPositionX() > wb.right * 0.2) {
      py->setPositionX(wb.right * 0.2);
    }
  }

  if (pm->nextPos.y > wb.top * 0.6) {
    batch->setPositionY((wb.top * 0.6 - pm->nextPos.y) * 0.8);
  } else {
    batch->setPositionY(0);
  }

  if (te->isStartTerrain() && pm->vel.x > 0) {
    ss->speedIncreaseTimer += dt;
    if (ss->speedIncreaseTimer > ss->speedIncreaseInterval) {
      ss->speedIncreaseTimer = 0;
      pm->maxSpeed.x += 4;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::processPlayer(float dt) {

  auto pm=CC_GEC(PlayerMotion,_player,"f/CMove");
  auto py=CC_GEC(Player,_player,"f/CPixie");
  auto ps=CC_GEC(PlayerStats,_player,"f/CStats");

  if (pm->speed.x + P_ACCELERATION <= pm->maxSpeed.x) {
    pm->speed.x += P_ACCELERATION;
  } else {
    pm->speed.x = pm->maxSpeed.x;
  }

  pm->vel.x = pm->speed.x;

  switch (ps->state) {
    case kPlayerMoving: {
      pm->vel.y -= FORCE_GRAVITY;
      if (pm->isHasFloated()) { pm->setHasFloated(false); }
    }
    break;

    case kPlayerFalling: {
      if (pm->isFloating()) {
        pm->vel.x *= FLOATING_FRICTION;
        pm->vel.y -= FLOATNG_GRAVITY;
      } else {
        pm->vel.y -= FORCE_GRAVITY;
        pm->vel.x *= AIR_FRICTION;
        pm->setFloatingTimer( 0);
      }
    }
    break;

    case kPlayerDying: {
      pm->vel.y -= FORCE_GRAVITY;
      pm->vel.x = -pm->speed.x;
      py->setPositionX(py->getPositionX() + pm->vel.x);
    }
    break;

  }

  if (pm->isJumping()) {
    ps->state = kPlayerFalling;
    pm->vel.y += PLAYER_JUMP * 0.25;
    if (pm->vel.y > PLAYER_JUMP) { pm->setJumping(false); }
  }

  if (pm->vel.y < -TERMINAL_VELOCITY) {
    pm->vel.y = -TERMINAL_VELOCITY;
  }

  pm->nextPos.y = py->getPositionY() + pm->vel.y;

  if (pm->isFloating()) {
    pm->setFloatingTimer(pm->getFloatingTimer() + dt);
    if (pm->getFloatingTimer() > pm->getFloatingTimerMax()) {
      pm->setFloatingTimer(0);
      pm->setFloating(false);
    }
  }
}


NS_END




