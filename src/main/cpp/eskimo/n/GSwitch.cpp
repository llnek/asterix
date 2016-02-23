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
#include "core/ComObj.h"
#include "core/CCSX.h"
#include "GSwitch.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(eskimo)

//////////////////////////////////////////////////////////////////////////////
//
GSwitch* GSwitch::create() {
  auto s= cx::reifySprite("blank.png");
  return mc_new1(GSwitch,s);
}

//////////////////////////////////////////////////////////////////////////////
//
void GSwitch::initGSwitch(int direction, const c::Vec2 &position) {
  sprite->setDisplayFrame(cx::getSpriteFrame(
        "switch_" + s::to_string(direction) + ".png"));
  sprite->setPosition(position);
  CC_SHOW(sprite);
  this->direction = direction;
}


NS_EMD



