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

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "ParticleSpin.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(bazuka)

//////////////////////////////////////////////////////////////////////////////
//
ParticleSpin* ParticleSpin::create(const sstr &name, const c::Vec2 &pos) {
  auto pc = mc_new(ParticleSpin);
  pc->initWithSpriteFrameName(name);
  pc->autorelease();
  pc->setPosition(pos);
  return pc;
}

//////////////////////////////////////////////////////////////////////////////
//
void ParticleSpin::update(float dt) {

  auto initpos = this->getPosition();
  c::Vec2 finalpos;

  spinCounter += dt*4;

  finalpos.x = initpos.x + speed.x;
  speed.y += gravity.y;
  finalpos.y = initpos.y + speed.y + gravity.y;

  this->setPosition(finalpos);
  this->setRotation(CC_RADIANS_TO_DEGREES(spinCounter * speed.x));
}


NS_END

