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
#include "x2d/XLayer.h"
#include "Splash.h"
#include "MMenu.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
void Splash::decoUI() {

  auto title= cx::reifySprite("title.png");
  auto wb = cx::visBox();

  centerImage("game.bg");
  regoAtlas("game-pics");

  CC_POS2(title, wb.cx, wb.top * 0.8);
  addAtlasItem("game-pics", title);

    auto b = cx::reifyMenuText("btns","PLAY");
  auto f = []() { cx::prelude(); };
  auto x= mc_new1(MCX, f);

  b->setColor(c::Color3B::WHITE);
  XCFG()->scaleNode(b,36);
  b->setCallback(
      [=](c::Ref*) { cx::runEx( MMenu::reify(x)); });

  CC_POS2(b, wb.cx, wb.top * 0.2);
  addItem(cx::mkMenu(b));

}



NS_END

