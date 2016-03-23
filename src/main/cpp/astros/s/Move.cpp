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
NS_BEGIN(astros)


//////////////////////////////////////////////////////////////////////////////
//
void Move::preamble() {
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
  processShip(dt);
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::processAstro(float dt) {
  auto ship=CC_GEC(Ship,_player,"f/CPixie");
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto shipBoundingBox = ship->getBoundingBox();
  auto asteroidBoundingBox = astro->getBoundingBox();
  auto wb= cx::visBox();

  if (cx::collideN(ship,astro) &&
      !h->alive()) {
    gameLayer.removeAsteroid(this);
    SENDMSG("/game/player/lose");
  }

  if(astro->getPositionX() < wb.left -50) {
    gameLayer.removeAsteroid(this)
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::processShip(float dt) {
  auto h=CC_GEC(f::CHealth,_player,"f/CHealth");
  auto mv=CC_GEC(f::CMove,_player,"f/CMove");
  auto ship=CC_GEC(Ship,_player,"f/CPixie");
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto wb= cx::visBox();
  float sx=0;

  if (ship->engineOn) {
    mv->vel.y += ss->gameThrust;
    sx=ship->getPositionX()-25;
  } else {
    sx=ship->getPositionX()-250;
  }
  ss->emitter->setPosition(sx,ship->getPositionY());

  if(h->alive()) {
    h->hurt();
    ship->setOpacity(255- ship->getOpacity());
  }
  ship->setPosition(ship->getPositionX(), ship->getPositionY()+mv->vel.y);
  mv->vel.y += ss->gameGravity;
  if (ship->getPositionY() < wb.bottom ||
      ship->getPositionY() > wb.top) {
    SENDMSG("/game/player/lose");
  }

  onKeys(dt);
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::onKeys(float dt) {
}


NS_END




