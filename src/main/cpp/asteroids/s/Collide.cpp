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
#include "Collide.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(asteroids)

//////////////////////////////////////////////////////////////////////////////
//
void Collide::preamble() {
  shipNode = engine->getNodeList(ShipMotionNode().typeId());
  arenaNode = engine->getNodeList(ArenaNode().typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Collide::update(float dt) {

  if (MGMS()->isLive()) {
    checkMissilesRocks();
    checkShipBombs();
    checkShipRocks();
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::checkMissilesRocks() {
  auto p= MGMS()->getPool("Missiles");
  p->foreach([=](f::ComObj *m) {
    if (m->status && m->health > 0) {
      MGMS()->getPool("Astros3")->foreach([=](f::ComObj *a) {
        if (a->status && cx::collide(m,a)) {
          m->hurt();
          a->hurt();
        }
      });
    }
    if (m->status && m->health > 0) {
      MGMS()->getPool("Astros2")->foreach([=](f::ComObj *a) {
        if (a->status && cx::collide(m,a)) {
          m->hurt();
          a->hurt();
        }
      });
    }
    if (m->status && m->health > 0) {
      MGMS()->getPool("Astros1")->foreach([=](f::ComObj *a) {
        if (a->status && cx::collide(m,a)) {
          m->hurt();
          a->hurt();
        }
      });
    }
  });
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::checkShipBombs() {
  auto ship = CC_GNLF(Ship,shipNode,"ship");
  MGMS()->getPool("Lasers")->foreach([=](f::ComObj *b) {
    if (b->status && ship->status &&
        cx::collide(b,ship)) {
      ship->hurt();
      b->hurt();
    }
  });
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::checkShipRocks() {
  auto ship = CC_GNLF(Ship,shipNode,"ship");

  if (ship->status && ship->health > 0) {
    MGMS()->getPool("Astros3")->foreach([=](f::ComObj *a) {
      if (a->status && cx::collide(ship,a)) {
        ship->hurt();
        a->hurt();
      }
    });
  }

  if (ship->status && ship->health > 0) {
    MGMS()->getPool("Astros2")->foreach([=](f::ComObj *a) {
      if (a->status && cx::collide(ship,a)) {
        ship->hurt();
        a->hurt();
      }
    });
  }

  if (ship->status && ship->health > 0) {
    MGMS()->getPool("Astros1")->foreach([=](f::ComObj *a) {
      if (a->status && cx::collide(ship,a)) {
          ship->hurt();
          a->hurt();
      }
    });
  }

}


NS_END(asteroids)


