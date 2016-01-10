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
void Collide::checkXXX(f::XPool *p, f::ComObj *z) {
  p->foreach([=](f::ComObj *c) {
    if (z->status && c->status &&
        z->HP > 0 && c->HP > 0 &&
        cx::collide(z,c)) {
      z->hurt();
      c->hurt();
    }
  });
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::checkMissilesRocks() {
  MGMS()->getPool("Missiles")->foreach([=](f::ComObj *m) {
    this->checkXXX(MGMS()->getPool("Astros3"), m);
    this->checkXXX(MGMS()->getPool("Astros2"), m);
    this->checkXXX(MGMS()->getPool("Astros1"), m);
  });
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::checkShipBombs() {
  auto ship = CC_GNLF(Ship,shipNode,"ship");
  checkXXX(MGMS()->getPool("Lasers"), ship);
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::checkShipRocks() {
  auto ship = CC_GNLF(Ship,shipNode,"ship");
  checkXXX(MGMS()->getPool("Astros3"), ship);
  checkXXX(MGMS()->getPool("Astros2"), ship);
  checkXXX(MGMS()->getPool("Astros1"), ship);
}

NS_END(asteroids)


