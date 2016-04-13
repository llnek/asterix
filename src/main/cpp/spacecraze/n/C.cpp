// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

#include "core/CCSX.h"
#include "C.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(spacecraze)

//////////////////////////////////////////////////////////////////////////////
//
owner<Missile*> Missile::create(const sstr &fn) {
  auto z= mc_new(Missile);
  z->initWithSpriteFrameName(fn);
  z->autorelease();
  return z;
}

//////////////////////////////////////////////////////////////////////////////
//
owner<Alien*> Alien::create(int t) {
  auto z= mc_new1(Alien,t);
  z->initWithSpriteFrameName("sfenmy" + FTOS(t));
  z->autorelease();
  return z;
}

//////////////////////////////////////////////////////////////////////////////
//
owner<Ship*> Ship::create() {
  auto z= mc_new(Ship);
  z->initWithSpriteFrameName("sfgun");
  z->autorelease();
  return z;
}

//////////////////////////////////////////////////////////////////////////////
//
void spawnPlayer(not_null<ecs::Node*> node) {

  auto h=CC_GEC(f::CHealth,node.get(),"f/CHealth");
  auto s=CC_GEC(Ship,node.get(),"f/CPixie");
  auto wb = cx::visBox();

  cx::resurrect(node,wb.cx, wb.top * -0.1);
  s->setOpacity(255);
  s->setScale(1);
  h->enterGodMode();
  s->runAction(
      c::Sequence::create(
        c::EaseBackOut::create(
          c::MoveTo::create(1,
            CCT_PT(wb.cx, wb.top * 0.1))),
        c::CallFunc::create(
          [=]() { h->exitGodMode(); }),
        CC_NIL));
}

//////////////////////////////////////////////////////////////////////////////
//
j::json loadLevel(int n) {
  //char level_file[64] = {0}; //::sprintf(level_file, "Level%02d.xml", MGMS()->getLevel());
  sstr fp = n < 10 ? "0" : "";
  return cx::readJson("misc/Level"+fp+FTOS(n)+".json");
}


NS_END


