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

#include "x2d/GameScene.h"
#include "core/CCSX.h"
#include "Collide.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////
//
void Collide::preamble() {
  _ship = _engine->getNodes("f/CGesture")[0];
  _arena = _engine->getNodes("n/GVars")[0];
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
  bombs->foreach([=](f::Poolable* b) {
    mss->foreach([=](f::Poolable* m) {
      auto e2= (ecs::Node*)b;
      auto e1= (ecs::Node*)m;
      auto s2= CC_GEC(f::CDraw,e2,"f/CDraw");
      auto s1= CC_GEC(f::CDraw,e1,"f/CDraw");
      if (e2->status() &&
          e1->status() &&
          cx::collide(s2,s1)) {
        auto h2= CC_GEC(f::CHealth,e2,"f/CHealth");
        auto h1= CC_GEC(f::CHealth,e1,"f/CHealth");
        h2->hurt();
        h1->hurt();
      }
    });
  });
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::checkMissilesAliens() {
  auto enemies= MGMS()->getPool("Baddies");
  auto mss= MGMS()->getPool("Missiles");
  enemies->foreach([=](f::Poolable* en) {
    mss->foreach([=](f::Poolable* b) {
      auto e2= (ecs::Node*)b;
      auto e1= (ecs::Node*)en;
      auto s2= CC_GEC(f::CDraw,e2,"f/CDraw");
      auto s1= CC_GEC(f::CDraw,e1,"f/CDraw");
      if (e2->status() &&
          e1->status() &&
          cx::collide(s1,s2)) {
        auto h2= CC_GEC(f::CHealth,e2,"f/CHealth");
        auto h1= CC_GEC(f::CHealth,e1,"f/CHealth");
        h2->hurt();
        h1->hurt();
      }
    });
  });
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::checkShipBombs() {
  auto sp = CC_GEC(Ship, _ship, "f/CDraw");
  auto bombs = MGMS()->getPool("Bombs");

  if (_ship->status())
    bombs->foreach([=](f::Poolable* b) {
      auto e2= (ecs::Node*)b;
      auto s2= CC_GEC(f::CDraw,e2,"f/CDraw");
      if (e2->status() &&
          cx::collide(s2, sp)) {
        auto h2= CC_GEC(f::CHealth,_ship,"f/CHealth");
        auto h1= CC_GEC(f::CHealth,e2,"f/CHealth");
        h2->hurt();
        h1->hurt();
      }
    });
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::checkShipAliens() {
  auto enemies= MGMS()->getPool("Baddies");
  auto sp= CC_GEC(Ship, _ship, "f/CDraw");

  if (_ship->status())
    enemies->foreach([=](f::Poolable* en) {
      auto e2= (ecs::Node*)en;
      auto s2= CC_GEC(f::CDraw,e2,"f/CDraw");
      if (e2->status() &&
          cx::collide(s2, sp)) {
        auto h2= CC_GEC(f::CHealth,_ship,"f/CHealth");
        auto h1= CC_GEC(f::CHealth,e2,"f/CHealth");
        h2->hurt();
        h1->hurt();
      }
    });
}


NS_END



