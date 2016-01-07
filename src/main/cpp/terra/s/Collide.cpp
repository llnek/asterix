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
#include "core/CCSX.h"
#include "Collide.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////
//
void Collide::preamble() {
  ArenaNode a;
  ShipNode n;

  arenaNode = engine->getNodeList(a.typeId());
  shipNode = engine->getNodeList(n.typeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Collide::update(float dt) {

  if (MGMS()->isLive()) {
    checkMissilesAliens();
    checkShipAliens();
    checkShipBombs();
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::checkMissilesBombs() {
  auto mss = MGMS()->getPool("Missiles");
  auto bombs = MGMS()->getPool("Bombs");
  bombs->foreach([=](f::ComObj* b) {
    mss->foreach([=](f::ComObj* m) {
      if (b->status &&
          m->status &&
          cx::collide(b, m)) {
        m->hurt();
        b->hurt();
      }
    });
  });
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::checkMissilesAliens() {
  auto enemies= MGMS()->getPool("Baddies");
  auto mss= MGMS()->getPool("Missiles");
  enemies->foreach([=](f::ComObj* en) {
    mss->foreach([=](f::ComObj* b) {
      if (en->status &&
          b->status &&
          cx::collide(en, b)) {
        en->hurt();
        b->hurt();
      }
    });
  });
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::checkShipBombs() {
  auto ship= CC_GNLF(Ship, shipNode, "ship");
  auto bombs = MGMS()->getPool("Bombs");

  if (!ship->status) { return; }
  bombs->foreach([=](f::ComObj* b) {
    if (b->status &&
        cx::collide(b, ship)) {
      ship->hurt();
      b->hurt();
    }
  });
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::checkShipAliens() {
  auto ship= CC_GNLF(Ship, shipNode, "ship");
  auto enemies= MGMS()->getPool("Baddies");

  if (! ship->status) { return; }
  enemies->foreach([=](f::ComObj* en) {
    if (en->status &&
        cx::collide(en, ship)) {
      ship->hurt();
      en->hurt();
    }
  });
}


NS_END(terra)



