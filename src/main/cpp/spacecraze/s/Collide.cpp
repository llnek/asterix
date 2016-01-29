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
#include "Collide.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(spacecraze)


//////////////////////////////////////////////////////////////////////////////
//
void Collide::preamble() {
  aliens = engine->getNodeList(AlienNode().typeId());
  ships = engine->getNodeList(ShipNode().typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Collide::update(float dt) {
  if (MGMS()->isLive()) {
    checkMissiles();
    checkBombs();
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::checkMissiles() {
  auto ms = MGMS()->getPool("Missiles")->list();
  auto sq = CC_GNLF(AlienSquad,aliens,"squad");
  auto ss= sq->aliens->list();

  F__LOOP(it,ss) {
    auto a= *it;
    F__LOOP(it2,ms) {
      auto m= *it2;
      if (a->status && m->status) {
        if (cx::collide(a,m)) {
          a->hurt();
          m->hurt();
        }
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::checkBombs() {
  auto bs = MGMS()->getPool("Bombs")->list();
  auto ship = CC_GNLF(Ship,ships,"ship");

  if (ship->godMode) { return;}

  F__LOOP(it,bs) {
    auto b= *it;
    if (b->status && ship->status ) {
      if (cx::collide(b,ship)) {
        ship->hurt();
        b->hurt();
      }
    }
  }
}



NS_END





