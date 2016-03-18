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

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(flappy)

//////////////////////////////////////////////////////////////////////////////
//
void addDragon(f::XNode *node) {
  auto sp = cx::reifySprite("dhch_1");
  auto wb= cx::visBox();

  sp->setPosition(wb.right * 0.2, HTV(wb.top));
  node->addAtlasItem("dhtex", sp, E_LAYER_PLAYER);

  auto anim= CC_ACAC()->getAnimation("dragonFlying");
  sp->runAction(
      c::RepeatForever::create(c::Animate::create(anim)));

  auto seq = c::Sequence::create(
      c::EaseSineOut::create(
        c::MoveBy::create(
          anim->getDuration()/2, c::Vec2(0, 10))),
      c::EaseSineOut::create(
        c::MoveBy::create(
          anim->getDuration()/2, c::Vec2(0, -10))),
      CC_NIL);

  sp->runAction(c::RepeatForever::create(seq));
}



NS_END




