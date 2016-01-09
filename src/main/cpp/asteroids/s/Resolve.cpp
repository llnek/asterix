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

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Resolve.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(asteroids)

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::preamble() {
  shipNode = engine->getNodeList(ShipMotionNode().typeId());
  arenaNode = engine->getNodeList(ArenaNode().typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Resolve::update(float dt) {

  if (MGMS()->isLive()) {
    checkMissiles();
    checkLasers();
    checkAstros();
    checkShip();
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::checkMissiles() {
  auto B = MGMS()->getEnclosureBox();
  MGMS()->getPool("Missiles")->foreach([=](f::ComObj *m) {
    if (m->status) {
      if (m->health <= 0 ||
          cx::outOfBound(cx::bbox4(m), B)) {
      m->deflate();
    }}
  });
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::checkLasers() {
  auto B= MGMS()->getEnclosureBox();
  MGMS()->getPool("Lasers")->foreach([=](f::ComObj *b) {
    if (b->status) {
      if (b->health <= 0 ||
          cx::outOfBound(cx::bbox4(b), B)) {
      b->deflate();
    }}
  });
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::checkAstros() {
  MGMS()->getPool("Astros1")->foreach([=](f::ComObj *a) {
    if (a->status && a->health <= 0) {
      auto msg= j::json({ {"score", a->score} });
      SENDMSGEX("/game/players/earnscore", &msg);
      engine->createAsteroids(a->rank +1);
      a->deflate();
    }
  });
  MGMS()->getPool("Astros2")->foreach([=](f::ComObj *a) {
    if (a->status && a->health <= 0) {
      auto msg= j::json({ {"score", a->score} });
      SENDMSGEX("/game/players/earnscore", &msg);
      engine->createAsteroids(a->rank +1);
      a->deflate();
    }
  });
  MGMS()->getPool("Astros3")->foreach([=](f::ComObj *a) {
    if (a->status && a->health <= 0) {
      auto msg= j::json({ {"score", a->score} });
      SENDMSGEX("/game/players/earnscore", &msg);
      a->deflate();
    }
  });
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::checkShip() {
  auto ship = CC_GNLF(Ship,shipNode,"ship");
  if (ship->status && ship->health <= 0) {
    ship->deflate();
    SENDMSG("/game/players/killed");
  }
}



NS_END(asteroids)


