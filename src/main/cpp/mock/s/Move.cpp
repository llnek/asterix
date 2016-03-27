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
  _terrain= _engine->getNodes("n/Terrain")[0];
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

  if (py->getPositionY() < -cx::getHeight(py) ||
      py->getPositionX() < -HTV(cx::getWidth(py)) ) {
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
void Move::processPlayer(float dt) {

  if (mv.speed.x + P_ACCELERATION <= mv.maxSpeed.x) {
    mv.speed.x += P_ACCELERATION;
  } else {
    mv.speed.x = mv.maxSpeed.x;
  }

  mv.vel.x = mv.speed.x;

  switch (ps->state) {
    case kPlayerMoving:
      if (mv->hasFloated) { mv->hasFloated = false; }
      mv->vel.y -= FORCE_GRAVITY;
    break;

    case kPlayerFalling:
      if (mv->isFloating() ) {
        mv->vel.x *= FLOATING_FRICTION;
        mv->vel.y -= FLOATNG_GRAVITY;
      } else {
        mv->vel.y -= FORCE_GRAVITY;
        mv->vel.x *= AIR_FRICTION;
        mv->floatingTimer = 0;
      }
    break;

    case kPlayerDying:
      mv->vel.y -= FORCE_GRAVITY;
      mv->vel.x = -mv->speed.x;
      py->setPositionX(py->getPositionX() + mv->vel.x);
    break;
  }

  if (mv->isJumping() ) {
    mv->vel.y += PLAYER_JUMP * 0.25;
    ps->state = kPlayerFalling;
    if (mv->vel.y > PLAYER_JUMP) { mv->setJumping(false); }
  }

  if (mv->vel.y < -TERMINAL_VELOCITY) { mv->vel.y = -TERMINAL_VELOCITY; }

  mv->nextPos.y = py->getPositionY() + mv->vel.y;

  if (mv->isFloating() ) {
    mv->floatingTimer += dt;
    if (mv->floatingTimer > mv->floatingTimerMax) {
      mv->floatingTimer = 0;
      mv->setFloating(false);
    }
  }

}


NS_END




