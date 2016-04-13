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

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "BackDrop.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(spacecraze)

//////////////////////////////////////////////////////////////////////////////
//
void BackDrop::decoUI() {

  auto num= CC_CSV(c::Integer, "MAX+STARS");
  auto wb= cx::visBox();

  centerImage("game.bg");
  regoAtlas("game-pics");

  //sprinkle stars
  for (auto i = 0; i < num; ++i) {
    auto s = cx::reifySprite("star");
    cx::randomPos(s);
    S__ADD(_stars,s);
    if (i==0) {
      this->_sz= CC_CSIZE(s); }
    addAtlasItem("game-pics", s);
  }

  scheduleUpdate();
}

//////////////////////////////////////////////////////////////////////////////
//
void BackDrop::update(float dt) {
  auto di= CC_CSV(c::Float, "star+speed+inc");
  auto d= CC_CSV(c::Float, "star+speed");
  auto wb = cx::visBox();

  F__LOOP(it, _stars) {
    auto &s= *it;
    auto y= s->getPositionY() - d;

    if(y < -HHZ(_sz)) {
      y = wb.top + HHZ(_sz);
    }

    s->setPositionY(y);
    d += di;
  }
}



NS_END



