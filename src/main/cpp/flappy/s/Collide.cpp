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

//////////////////////////////////////////////////////////////////////////////

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Collide.h"
#include "n/Dragon.h"
#include "n/Tower.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(flappy)


//////////////////////////////////////////////////////////////////////////////
//
void Collide::preamble() {
  _shared= _engine->getNodes("n/GVars")[0];
}

//////////////////////////////////////////////////////////////////////////////
//
bool Collide::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::process(float dt) {
  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  // first find out which tower is right in front
  auto frontTower = ss->towers->getFrontTower();
  // fetch the bounding boxes of the respective sprites
  auto dragonAABB = CC_BBOX(ss->dragon->node);
  auto lowerTowerAABB = CC_BBOX(frontTower.lowerSprite);
  auto upperTowerAABB = CC_BBOX(frontTower.upperSprite);

  // if the respective rects intersect, we have a collision
  if (dragonAABB.intersectsRect( lowerTowerAABB) ||
      dragonAABB.intersectsRect(upperTowerAABB)) {
    // dragon must die
    ss->dragon->dragonDeath();
    SENDMSG("/game/player/lose");
  }
  else
  if (abs(lowerTowerAABB.getMidX() - dragonAABB.getMidX()) <= MAX_SCROLLING_SPEED/2 ) {
    auto msg= j::json({
        {"score", 1}
        });
    SENDMSGEX("/game/player/earnscore", &msg);
  }
}

NS_END




