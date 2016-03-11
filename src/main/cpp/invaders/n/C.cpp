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
#include "C.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////////
  //auto gz= XCFG()->gameSize();
  //auto wz= cx::visRect();
  //vel.x=0;
  //vel.y= -50.0f * wz.size.height / gz.height;

//////////////////////////////////////////////////////////////////////////////
//
void Explosion::inflate(float x, float y) {
  auto anim= CC_ACAC()->getAnimation("boom!");
  f::CDraw::inflate(x,y);
  node->runAction(
    c::Sequence::createWithTwoActions(c::Animate::create(anim),
    c::CallFunc::create([=]() { this->deflate(); })));
}

//////////////////////////////////////////////////////////////////////////////
Looper::~Looper() {
  CC_DROP(timer0)
  CC_DROP(timer1)
  CC_DROP(timer7)
}

//////////////////////////////////////////////////////////////////////////////
//
  //auto gz= XCFG()->gameSize();
  //auto wz= cx::visRect();
  //vel.x= 0;
  //vel.y= 150.0f * wz.size.height / gz.height;


NS_END


