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
  _aliens = _engine->getNodes("n/AlienSquad")[0];
  _ship = _engine->getNodes("f/CGesture")[0];
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
  auto po = MGMS()->getPool("Missiles");
  auto pa = MGMS()->getPool("Aliens");
  cx::testCollisions(po,pa);
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::checkBombs() {

  auto h=CC_GEC(f::CHealth,_ship,"f/CHealth");
  if (h->isGod()) {
  return; }

  cx::testCollisions(
      MGMS()->getPool("Bombs"),_ship);
}


NS_END





