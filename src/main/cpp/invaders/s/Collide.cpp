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

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////
//
void Collide::preamble() {
  aliens= engine->getNodeList(AlienMotionNode().typeId());
  ship= engine->getNodeList(ShipMotionNode().typeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Collide::update(float dt) {

  // 1. get rid of all colliding bombs & missiles.
  //this.checkMissilesBombs();
  // 2. kill aliens
  checkMissilesAliens();

  // 3. ship ok?
  checkShipBombs();
  // 4. overruned by aliens ?
  checkShipAliens();

  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::checkMissilesBombs() {

  auto mss = MGMS()->getPool("missiles");
  auto bbs = MGMS()->getPool("bombs");
  auto m = mss->list();
  auto b= bbs->list();

  F__LOOP(it, m) {
    F__LOOP(it2, b) {

      auto &e2 = *it2;
      auto &e= *it;
      if (e2->status &&
          e->status &&
          cx::collide(e2,e)) {
        e2->hurt();
        e->hurt();
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::checkMissilesAliens() {

  auto sqad= CC_GNLF(AlienSquad, aliens, "aliens");
  auto mss = MGMS()->getPool("missiles");
  auto c = sqad->list();
  auto c2 = mss->list();

  F__LOOP(it, c) {
    F__LOOP(it2, c2) {
      auto &e2= *it2;
      auto &e = *it;
      if (e->status && e2->status &&
          cx::collide(e,e2)) {
        e->hurt();
        e2->hurt();
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::checkShipBombs() {

  auto sp= CC_GNLF(Ship, ship, "ship");
  auto bbs= MGMS()->getPool("bombs");
  auto c= bbs->list();

  if (sp->status)
    F__LOOP(it, c) {
      auto &b = *it;
      if (b->status &&
          cx::collide(sp, b)) {
        sp->hurt();
        b->hurt();
      }
    }
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::checkShipAliens() {

  auto sqad= CC_GNLF(AlienSquad, aliens, "aliens");
  auto sp = CC_GNLF(Ship, ship, "ship");
  auto c = sqad->list();
  auto sz= sqad->size();

  if (sp->status)
    F__LOOP(it, c) {
      auto &a = *it;
      if (a->status &&
          cx::collide(sp, a)) {
        sp->hurt();
        a->hurt();
      }
    }
}



NS_END(invaders)


