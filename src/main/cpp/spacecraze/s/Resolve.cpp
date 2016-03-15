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

#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Resolve.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(spacecraze)


//////////////////////////////////////////////////////////////////////////////
//
void Resolve::preamble() {
  _aliens = _engine->getNodes("n/AlienSquad")[0];
  _ship = _engine->getNodes("f/CGesture")[0];
}

//////////////////////////////////////////////////////////////////////////////
//
bool Resolve::update(float dt) {
  if (MGMS()->isLive()) {
    checkMissiles();
    checkBombs();
    checkAliens();
    checkShip();
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::checkMissiles() {
  auto ms = MGMS()->getPool("Missiles");
  ms->foreach([=](f::ComObj *c) {
    if (!c->status || c->HP <=0) {
      c->deflate();
    }
  });
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::checkBombs() {
  auto ms = MGMS()->getPool("Bombs");
  ms->foreach([=](f::ComObj *c) {
    if (!c->status || c->HP <=0) {
      c->deflate();
    }
  });
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::checkAliens() {
  auto sq = CC_GNLF(AlienSquad,aliens,"squad");
  sq->aliens->foreach([=](f::ComObj *c) {
    if (c->status && c->HP <=0) {
      c->freeze();
      SENDMSGEX("/game/alien/killed", c);
    }
  });
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::checkShip() {
  auto ship = CC_GNLF(Ship,ships,"ship");

  if (ship->godMode) { return; }

  if (ship->status && ship->HP <= 0) {
    ship->freeze();
    SENDMSG("/game/player/killed");
  }
}


NS_END





