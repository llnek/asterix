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
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Collide.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(asteroids)

//////////////////////////////////////////////////////////////////////////////
//
void Collide::preamble() {
  _ship = _engine->getNodes("f/CGesture")[0];
  _arena = _engine->getNodes("n/GVars")[0];
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
void Collide::checkXXX(f::FPool *po, ecs::Node *z) {
  po->foreach([=](f::Poolable *p) {
      auto e= (ecs::Node*) p;
      auto h1=CC_GEC(f::CHealth,e,"f/CHealth");
      auto h2=CC_GEC(f::CHealth,z,"f/CHealth");
      auto s1=CC_GEC(f::CPixie,e,"f/CPixie");
      auto s2=CC_GEC(f::CPixie,z,"f/CPixie");
    if (z->status() && e->status() &&
        h1->alive() && h2->alive() &&
        cx::collide(s1,s2)) {
      h2->hurt();
      h1->hurt();
    }
  });
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::checkMissilesRocks() {
  auto po= MGMS()->getPool("Missiles");
  po->foreach([=](f::Poolable *p) {
      auto e= (ecs::Node*) p;
    this->checkXXX(MGMS()->getPool("Astros3"), e);
    this->checkXXX(MGMS()->getPool("Astros2"), e);
    this->checkXXX(MGMS()->getPool("Astros1"), e);
  });
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::checkShipBombs() {
  checkXXX(MGMS()->getPool("Lasers"), _ship);
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::checkShipRocks() {
  checkXXX(MGMS()->getPool("Astros3"), _ship);
  checkXXX(MGMS()->getPool("Astros2"), _ship);
  checkXXX(MGMS()->getPool("Astros1"), _ship);
}


NS_END


