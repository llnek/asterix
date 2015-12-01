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
#include "n/GNodes.h"
#include "Collide.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////
//
Collide::Collide(not_null<EFactory*> f,
    not_null<c::Dictionary*> d)

  : f::BaseSystem<EFactory>(f,d) {

  SNPTR(aliens)
  SNPTR(engine)
  SNPTR(ships)
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::addToEngine(not_null<a::Engine*> e) {
  //CCLOG("adding system: Collide");
  AlienMotionNode a;
  ShipMotionNode s;

  aliens= e->getNodeList(a.TypeId());
  ships= e->getNodeList(s.TypeId());
  //CCLOG("added system: Collide");
}

//////////////////////////////////////////////////////////////////////////
//
bool Collide::onUpdate(float dt) {
  auto enemies= aliens->head;
  auto ship = ships->head;

  // 1. get rid of all colliding bombs & missiles.
  //this.checkMissilesBombs();
  // 2. kill aliens
  checkMissilesAliens(enemies);

  if (NNP(ship)) {
    // 3. ship ok?
    checkShipBombs(ship);
    // 4. overruned by aliens ?
    if (NNP(enemies)) {
      checkShipAliens(enemies, ship);
    }
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////
//
bool Collide::maybeCollide(not_null<f::ComObj*> a,
    not_null<f::ComObj*> b) {

  return cx::collideN(a->sprite, b->sprite);
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::checkMissilesBombs() {

  auto mss = MGMS()->getPool("missiles");
  auto bbs = MGMS()->getPool("bombs");
  auto m = mss->elements();
  auto b= bbs->elements();

  F__LOOP(it, m) {
    F__LOOP(it2, b) {

      auto e2 = *it2;
      auto e= *it;
      if (e2->status &&
          e->status &&
          maybeCollide(e2,e)) {
        e2->hurt();
        e->hurt();
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::checkMissilesAliens(not_null<a::Node*> node) {

  auto sqad= CC_GNF(AlienSquad, node, "aliens");
  auto mss = MGMS()->getPool("missiles");
  auto c = sqad->elements();
  auto c2 = mss->elements();

  F__LOOP(it, c) {
    F__LOOP(it2, c2) {
      auto e2= *it2;
      auto e = *it;
      if (e->status && e2->status &&
          maybeCollide(e,e2)) {
        e->hurt();
        e2->hurt();
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::checkShipBombs(not_null<a::Node*> node) {

  auto ship= CC_GNF(Ship, node, "ship");
  auto bbs= MGMS()->getPool("bombs");
  auto c= bbs->elements();

  if (ship->status)
    F__LOOP(it, c) {
      auto b = *it;
      if (b->status &&
          maybeCollide(ship, b)) {
        ship->hurt();
        b->hurt();
      }
    }
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::checkShipAliens(not_null<a::Node*> anode,
    not_null<a::Node*> snode) {

  auto sqad= CC_GNF(AlienSquad, anode, "aliens");
  auto ship = CC_GNF(Ship, snode, "ship");
  auto c = sqad->elements();
  auto sz= sqad->size();

  if (ship->status)
    F__LOOP(it, c) {
      auto a = *it;
      if (a->status &&
          maybeCollide(ship, a)) {
        ship->hurt();
        a->hurt();
      }
    }
}



NS_END(invaders)


