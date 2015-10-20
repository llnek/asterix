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

#include "x2d/MainGame.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "n/gnodes.h"
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
void Collide::AddToEngine(not_null<a::Engine*> e) {
  CCLOG("adding system: Collide");
  AlienMotionNode a;
  ShipMotionNode s;

  aliens= e->GetNodeList(a.TypeId());
  ships= e->GetNodeList(s.TypeId());
  CCLOG("added system: Collide");
}

//////////////////////////////////////////////////////////////////////////
//
bool Collide::OnUpdate(float dt) {
  auto enemies= aliens->head;
  auto ship = ships->head;

  // 1. get rid of all colliding bombs & missiles.
  //this.checkMissilesBombs();
  // 2. kill aliens
  CheckMissilesAliens(enemies);

  if (NNP(ship)) {
    // 3. ship ok?
    CheckShipBombs(ship);
    // 4. overruned by aliens ?
    if (NNP(enemies)) {
      CheckShipAliens(enemies, ship);
    }
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////
//
bool Collide::MaybeCollide(not_null<f::ComObj*> a,
    not_null<f::ComObj*> b) {

  return cx::CollideN(a->sprite, b->sprite);
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::CheckMissilesBombs() {

  auto mss = MGMS()->GetPool("missiles");
  auto bbs = MGMS()->GetPool("bombs");
  auto m = mss->Elements();
  auto b= bbs->Elements();

  for (auto it = m.begin(); it != m.end(); ++it) {
    for (auto it2 = b.begin(); it2 != b.end(); ++it2) {
      auto e2 = *it2;
      auto e= *it;
      if (e2->status &&
          e->status &&
          MaybeCollide(e2,e)) {
        e2->Hurt();
        e->Hurt();
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::CheckMissilesAliens(not_null<a::Node*> node) {

  auto sqad= a::NodeFld<AlienSquad>(node, "aliens");
  auto mss = MGMS()->GetPool("missiles");
  auto c = sqad->Elements();
  auto c2 = mss->Elements();

  for (auto it = c.begin(); it != c.end(); ++it) {
    for (auto it2 = c2.begin(); it2 != c2.end(); ++it2) {
      auto e2= *it2;
      auto e = *it;
      if (e->status && e2->status &&
          MaybeCollide(e,e2)) {
        e->Hurt();
        e2->Hurt();
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::CheckShipBombs(not_null<a::Node*> node) {

  auto ship= a::NodeFld<Ship>(node, "ship");
  auto bbs= MGMS()->GetPool("bombs");
  auto c= bbs->Elements();

  if (ship->status)
    for (auto it = c.begin(); it != c.end(); ++it) {
      auto b = *it;
      if (b->status &&
          MaybeCollide(ship, b)) {
        ship->Hurt();
        b->Hurt();
      }
    }
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::CheckShipAliens(not_null<a::Node*> anode,
    not_null<a::Node*> snode) {

  auto sqad= a::NodeFld<AlienSquad>(anode, "aliens");
  auto ship = a::NodeFld<Ship>(snode, "ship");
  auto c = sqad->Elements();
  auto sz= sqad->Size();

  if (ship->status)
    for (auto it = c.begin(); it != c.end(); ++it) {
      auto a = *it;
      if (a->status &&
          MaybeCollide(ship, a)) {
        ship->Hurt();
        a->Hurt();
      }
    }
}



NS_END(invaders)


