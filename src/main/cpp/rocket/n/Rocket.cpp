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
#include "Rocket.h"
#include "C.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(rocket)

//////////////////////////////////////////////////////////////////////////////
//
RocketMotion::RocketMotion()
  : CMove() {
  targetRotation=0;
  dr=0;
  ar=0;
  vr=0;
  rotationSpring= 0.1;
  rotationDamping= 0.8;
  pivot= c::Vec2(-1,-1);
  rotationOrientation= ROTATE_NONE;
}

//////////////////////////////////////////////////////////////////////////////
//
void rocketReset(not_null<Rocket*> r, not_null<RocketMotion*> m) {

  m->rotationOrientation = ROTATE_NONE;
  m->targetRotation = -90;
  m->pivot = c::Vec2(-1,1);
  m->speed.x = 80;
  m->speed.y = 80;
  r->node->setRotation(-90);
  auto angle = CC_DEGREES_TO_RADIANS(r->node->getRotation());
  m->vel = c::Vec2(m->speed.x * cos(angle), -m->speed.y * sin(angle));
}

//////////////////////////////////////////////////////////////////////////////
//
void rocketSelect(not_null<Rocket*> r, bool flag) {
  auto f= flag ? "rocket_on.png" : "rocket.png";
    SCAST(c::Sprite*,r->node)->setDisplayFrame(cx::getSpriteFrame(f));
}


NS_END


