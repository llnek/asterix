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

#include "core/CCSX.h"
#include "Collide.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
//
Collide::Collide(not_null<a::Engine*> e, not_null<c::Dictionary*> d)

  : f::BaseSystem<EFactory>(e, d) {

  SNPR(ships)
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::addToEngine(not_null<a::Engine*> e) {
  ShipMotionNode n;
  ships = e->getNodeList(n.typeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Collide::collide(f::ComObj* a, f::ComObj* b) {
  return cx::collide0(a->sprite, b->sprite);
}

//////////////////////////////////////////////////////////////////////////
//
bool Collide::onUpdate(float dt) {
  auto node = ships->head;

  if (MGMS()->isLive() && NNP(node)) {
    checkMissilesAliens();
    checkShipAliens(node);
    checkShipBombs(node);
    //this.checkMissilesBombs();
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
          this->collide(b, m)) {
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
          this->collide(en, b)) {
        en->hurt();
        b->hurt();
      }
    });
  });
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::checkShipBombs(a::Node* node) {
  auto bombs = MGMS()->getPool("Bombs");
  auto ship= node->ship;

  if (!ship->status) { return; }
  bombs->foreach([=](f::ComObj* b) {
    if (b->status &&
        this->collide(b, ship)) {
      ship->hurt();
      b->hurt();
    }
  });
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::checkShipAliens(a::Node* node) {
  auto enemies= MGMS()->getPool("Baddies");
  auto ship= node->ship;

  if (! ship->status) { return; }
  enemies->foreach([=](f::ComObj* en) {
    if (en->status &&
        this->collide(en, ship)) {
      ship->hurt();
      en->hurt();
    }
  });
}


NS_END(terra)



