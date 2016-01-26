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
  ships = engine->getNodeList(ShipMotionNode().typeId());
  arenas = engine->getNodeList(ArenaNode().typeId());
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
void Resolve::checkXXX(f::XPool *p) {
  auto B= MGMS()->getEnclosureBox();
  p->foreach([=](f::ComObj *b) {
    if (b->status) {
      if (b->HP <= 0 ||
          cx::outOfBound(cx::bbox4(b), B)) {
      b->deflate();
    }}
  });
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::checkAstrosXXX(f::XPool *p, bool cr) {
  p->foreach([=](f::ComObj *a) {
    if (a->status && a->HP <= 0) {
      auto msg= j::json({ {"score", a->score} });
      SENDMSGEX("/game/players/earnscore", &msg);
      if (cr) {
        SCAST(GEngine*,engine)->createAsteroids(
                                                SCAST(Asteroid*,a)->rank +1);
      }
      a->deflate();
    }
  });
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::checkShip() {
  auto ship = CC_GNLF(Ship,ships,"ship");
  if (ship->status && ship->HP <= 0) {
    ship->deflate();
    SENDMSG("/game/players/killed");
  }
}



NS_END(asteroids)


