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
#include "Resolve.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(asteroids)

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::preamble() {
  _arena = _engine->getNodes("n/GVars")[0];
  _ship = _engine->getNodes("n/Ship")[0];
}

//////////////////////////////////////////////////////////////////////////////
//
bool Resolve::update(float dt) {

  if (MGMS()->isLive()) {
    checkXXX(MGMS()->getPool("Missiles"));
    checkXXX(MGMS()->getPool("Lasers"));
    checkAstrosXXX(MGMS()->getPool("Astros1"),true);
    checkAstrosXXX(MGMS()->getPool("Astros2"),true);
    checkAstrosXXX(MGMS()->getPool("Astros3"),false);
    checkShip();
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::checkXXX(f::FPool *po) {
  auto B= MGMS()->getEnclosureBox();
  po->foreach([=](f::Poolable *b) {
      auto e= (ecs::Node*)b;
      auto h=CC_GEC(f::CHealth,e,"f/CHealth");
      auto s=CC_GEC(f::CDraw,e,"f/CDraw");
    if (e->status()) {
      if (!h->alive() ||
          cx::outOfBound(cx::bbox4(s), B)) {
      s->deflate();
      e->yield();
    }}
  });
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::checkAstrosXXX(f::FPool *po, bool cr) {
  po->foreach([=](f::Poolable *p) {
    auto e= (ecs::Node*)p;
    auto h=CC_GEC(f::CHealth,e,"f/CHealth");
    auto s=CC_GEC(f::CDraw,e,"f/CDraw");
    auto a=CC_GEC(Asteroid,e,"n/Asteroid");
    if (e->status() && !h->alive()) {
      auto msg= j::json({ {"score", a->value} });
      SENDMSGEX("/game/players/earnscore", &msg);
      if (cr) {
        SCAST(GEngine*,engine)->createAsteroids(a->rank +1);
      }
      s->deflate();
      e->yield();
    }
  });
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::checkShip() {
  auto h = CC_GEC(f::CHealth, _ship,"f/CHealth");
  auto s= CC_GEC(Ship, _ship,"n/Ship");
  if (_ship->status() && !h->alive()) {
    _ship->yield();
    s->deflate();
    SENDMSG("/game/players/killed");
  }
}



NS_END


