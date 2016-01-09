// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2014, Ken Leung. All rights reserved.

#include "core/CCSX.h"
#include "Stage.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(asteroids)

//////////////////////////////////////////////////////////////////////////////
//
void Stage::preamble() {
  /*
  sh.pools.Missiles = xpool.reify();
  sh.pools.Lasers = xpool.reify();
  sh.pools.Astros3 = xpool.reify();
  sh.pools.Astros2 = xpool.reify();
  sh.pools.Astros1 = xpool.reify();
  this.initAsteroidSizes();
  this.initPlayerSize();
  this.initUfoSize();
  sh.factory.createAsteroids(csts.P_AS1);
  sh.factory.createShip();
  */
  auto nl= engine->getNodeList(ArenaNode().typeId());
  auto ss= CC_GNLF(GVars,nl,"slots");
  ss->playerSize = cx::calcSize("rship_0.png");
  ss->ufoSize = cx::calcSize("ufo.png");
  ss->astro3 = cx::calcSize("rock_small.png");
  ss->astro2 = cx::calcSize("rock_med.png");
  ss->astro1 = cx::calcSize("rock_large.png");
}

//////////////////////////////////////////////////////////////////////////////
//
bool Stage::update(float dt) {
  return true;
}

NS_END(asteroids)

