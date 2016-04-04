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
#include "n/Terrain.h"
#include "n/Player.h"
#include "Move.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(victorian)

//////////////////////////////////////////////////////////////////////////////
//
void Move::preamble() {
  _terrain= _engine->getNodes("f/CAutoma")[0];
  _player= _engine->getNodes("f/CGesture")[0];
  _shared= _engine->getNodes("n/GVars")[0];
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::process(float dt) {
  auto pm=CC_GEC(PlayerMotion, _player,"f/CMove");
  auto ps=CC_GEC(PlayerStats, _player,"f/CStats");
  auto tn=CC_GEC(Terrain, _terrain,"f/CPixie");
  auto py=CC_GEC(Player, _player,"f/CPixie");
  auto ss=CC_GEC(GVars, _shared,"n/GVars");
  auto atlas=MGML()->getAtlas("game-pics");
  auto wb=cx::visBox();

  processPlayer(dt);
  tn->
  move(pm->vel.x);

  if (ps->state != kPlayerDying) {
    tn->checkCollision(_player);
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
    atlas->setPositionY((wb.top * 0.6 - pm->nextPos.y) * 0.8);
  } else {
    atlas->setPositionY(0);
  }

  if (ss->jam->isVisible()) {
    if (ss->jam->getPositionX() < -wb.right * 0.2) {
      ss->jam->stopAllActions();
      CC_HIDE(ss->jam);
    }
  }

}


//////////////////////////////////////////////////////////////////////////////
//
void Move::processPlayer(float dt) {
  auto pm=CC_GEC(PlayerMotion, _player,"f/CMove");
  auto ps=CC_GEC(PlayerStats, _player,"f/CStats");
  auto py=CC_GEC(Player, _player,"f/CPixie");
  auto ss=CC_GEC(GVars, _shared,"n/GVars");
  auto wb=cx::visBox();

  if (pm->speed.x + P_ACCELERATION <= pm->maxSpeed.x) {
    pm->speed.x += P_ACCELERATION;
  } else {
    pm->speed.x = pm->maxSpeed.x;
  }

  pm->vel.x = pm->speed.x;

  switch (ps->state) {
    case kPlayerMoving:
      pm->vel.y -= G_FORCE;
      if (pm->getHasFloated()) pm->setHasFloated(false);
    break;

    case kPlayerFalling:
      if (pm->isFloating() ) {
        pm->vel.x *= FLOATING_FRICTION;
        pm->vel.y -= FLOATNG_GRAVITY;
      } else {
        pm->vel.x *= AIR_FRICTION;
        pm->vel.y -= G_FORCE;
        pm->setFloatingTimer(0);
      }
    break;

    case kPlayerDying:
      pm->vel.y -= G_FORCE;
      pm->vel.x = -pm->speed.x;
      py->setPositionX(py->getPositionX() + pm->vel.x);
    break;
  }

  if (pm->isJumping()) {
    ps->state = kPlayerFalling;
    pm->vel.y += PLAYER_JUMP * 0.25;
    if (pm->vel.y > PLAYER_JUMP ) pm->setJumping(false);
  }

  if (pm->vel.y < -TERMINAL_VELOCITY) pm->vel.y = -TERMINAL_VELOCITY;

  pm->nextPos.y = py->getPositionY() + pm->vel.y;

  if (pm->vel.x * pm->vel.x < 0.01) pm->vel.x = 0;
  if (pm->vel.y * pm->vel.y < 0.01) pm->vel.y = 0;

  if (pm->isFloating() ) {
    pm->setFloatingTimer(pm->getFloatingTimer() + dt);
    if (pm->getFloatingTimer() > pm->getFloatingTimerMax() ) {
      pm->setFloating(py, false);
      pm->setFloatingTimer(0);
      cx::sfxPlay("falling");
    }
  }

}

//////////////////////////////////////////////////////////////////////////////
//
bool Move::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}


NS_END




