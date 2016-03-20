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
#include "core/COMP.h"
#include "core/CCSX.h"
#include "lib.h"
#include "Blast.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(blast)

//////////////////////////////////////////////////////////////////////////////
//
owner<Blast*> Blast::create(float radius, float duration) {
  auto b = f::reifyRefType<Blast>();

  b->_duration = duration;
  b->_radius = radius;

  // initially scale down completely
  b->setScale(0);
  b->drawDot(CC_ZPT, _radius, c::ccc4f(1, 0.34118, 0, 1));
  b->drawDot(CC_ZPT, _radius * 0.8, c::ccc4f(1, 0.68235, 0, 0.25));
  b->drawDot(CC_ZPT, _radius * 0.75, c::ccc4f(1, 0.68235, 0, 0.5));
  b->drawDot(CC_ZPT, _radius * 0.7, c::ccc4f(1, 0.68235, 0, 0.5));
  b->drawDot(CC_ZPT, _radius * 0.6, c::ccc4f(1, 0.83529, 0.40392, 0.25));
  b->drawDot(CC_ZPT, _radius * 0.55, c::ccc4f(1, 0.83529, 0.40392, 0.5));
  b->drawDot(CC_ZPT, _radius * 0.5, c::ccc4f(1, 0.83529, 0.40392, 0.5));
  b->drawDot(CC_ZPT, _radius * 0.4, c::ccc4f(1, 1, 1, 0.25));
  b->drawDot(CC_ZPT, _radius * 0.35, c::ccc4f(1, 1, 1, 0.75));
  b->drawDot(CC_ZPT, _radius * 0.3, c::ccc4f(1, 1, 1, 1));
  // scale-up, then wait for 'duration_' amount of seconds before cooling down
  b->runAction(
      c::Sequence::create(
        c::EaseSineOut::create(
          c::ScaleTo::create(0.25, 1)),
        c::DelayTime::create(_duration),
        c::CallFunc::create([=]() {
          b->coolDown();
          }),
        CC_NIL));

  return b;
}

//////////////////////////////////////////////////////////////////////////////
//
void Blast::cooldown() {
  // remove this blast in the next iteration
  _must_be_removed = true;
  // animate exit then remove with cleanup
  runAction(
      c::Sequence::create(
        c::EaseSineOut::create(
          c::ScaleTo::create(0.5, 0)),
        c::RemoveSelf::create(true),
        CC_NIL));
}


NS_END


