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

#include "core/CCSX.h"
#include "lib.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(spacecraze)

//////////////////////////////////////////////////////////////////////////////
//
void spawnPlayer(ecs::Node *node) {

  auto h=CC_GEC(f::CHealth,node,"f/CHealth");
  auto s=CC_GEC(Ship,node,"f/CPixie");
  auto wb = cx::visBox();

  s->inflate(wb.cx, wb.top * -0.1);
  h->reset();
  s->node->setOpacity(255);
  s->node->setScale(1);
  s->enterGodMode();
  s->node->runAction(
      c::Sequence::create(
        c::EaseBackOut::create(
          c::MoveTo::create(1,
            c::Vec2(wb.cx, wb.top * 0.1))),
        c::CallFunc::create(
          [=]() { h->exitGodMode(); }),
        CC_NIL));
  node->take();
}

//////////////////////////////////////////////////////////////////////////////
//
j::json loadLevel(int n) {
  //char level_file[64] = {0}; //::sprintf(level_file, "Level%02d.xml", MGMS()->getLevel());
  sstr fp = n < 10 ? "0" : "";
  return cx::readJson("pics/Level" + fp + s::to_string(n) + ".json");
}


NS_END


