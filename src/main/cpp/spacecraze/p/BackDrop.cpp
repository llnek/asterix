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

#include "BackDrop.h"

NS_BEGIN(spacecraze)

//////////////////////////////////////////////////////////////////////////////
//
owner<BackDrop*> BackDrop::reify() {
  auto s= f::reifyRefType<BackDrop>();
  s->decorate();
  return s;
}

//////////////////////////////////////////////////////////////////////////////
//
void BackDrop::decorate() {

  auto wb= cx::visBox();
  c::Size sz;

  centerImage("game.bg");
  regoAtlas("game-pics");

  //sprinkle stars
  for (auto i = 0; i < 15; ++i) {
    auto s = cx::reifySprite("star");
    if (i==0) {
      sz= s->getContentSize();
    }
    s->setPosition(cx::rand() * wb.right, cx.rand() * wb.top);
    stars.push_back(s);
    addAtlasItem("game-pics", s);
  }

  scheduleUpdate();
}

//////////////////////////////////////////////////////////////////////////////
//
void BackDrop::update(float dt) {
  auto speed = 10.0f;

  F__LOOP(it, stars) {
    auto s= *it;
    auto y= s->getPositionY() - speed;

    if(y < sz.height * -0.5) {
      y = wb.top + sz.height * 0.5;
    }

    s->setPositionY(y);
    speed += 0.5f;
  }
}



NS_END



