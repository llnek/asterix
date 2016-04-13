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
#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "ScrollingBgLayer.h"
#include "core/CCSX.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(bazuka)

//////////////////////////////////////////////////////////////////////////////
//
void ScrollingBgLayer::set(float speed) {

  auto bg = cx::createSprite("game.bg");
  auto wb= cx::visBox();

  bg->setPosition(wb.cx, wb.cy);
  addChild(bg, -1);

  hills = ScrollingBg::create("hills.png", speed * 0.3, 142);
  addChild(hills);

  treesNbush = ScrollingBg::create("treesNbush.png", speed * 0.5, 136);
  addChild(treesNbush);

  ground = ScrollingBg::create("ground.png", speed * 0.8, 0);
  addChild(ground);

  grass = ScrollingBg::create("grass.png", speed, 0);
  addChild(grass);
}

//////////////////////////////////////////////////////////////////////////////
//
void ScrollingBgLayer::sync() {
  hills->sync();
  treesNbush->sync();
  ground->sync();
  grass->sync();
}




NS_END



