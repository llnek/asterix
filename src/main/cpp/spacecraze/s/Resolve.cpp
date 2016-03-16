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
  cx::resolveNodes(MGMS()->getPool("Missiles"));
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::checkBombs() {
  cx::resolveNodes(MGMS()->getPool("Bombs"));
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::checkAliens() {
  auto po = MGMS()->getPool("Aliens");
  po->foreach([=](f::Poolable *p) {
    if (p->status()) {
      auto ht=CC_GEC(f::CHealth,p,"f/CHealth");
      if (!ht->alive()) {
        SENDMSGEX("/game/alien/killed", p);
      }
    }
  });
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::checkShip() {
  auto ht = CC_GEC(f::CHealth,_ship,"f/CHealth");
  if (!_ship->status() || ht->isGod()  ) {}
  else
  if (!ht->alive()) {
    SENDMSG("/game/player/killed");
  }
}


NS_END





