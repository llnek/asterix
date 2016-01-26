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

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "n/lib.h"
#include "MMenu.h"
#include "Splash.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
void Splash::demo() {
  auto ps= mapGridPos(0.75f);
  auto fm= "";

  // we scale down the icons to make it look nicer
  for (int i = 0; i < ps.size(); ++i) {
    // set up the grid icons
    if (i == 1 || i == 5 || i == 6 || i == 7)
    { fm= "x.png"; }
    else
    if (i == 0 || i == 4)
    { fm= "z.png"; }
    else
    { fm= "o.png"; }
    auto sp= cx::reifySprite(fm);
    auto bx= cx::vboxMID( ps[i]);
    sp->setScale(0.75f);
    sp->setPosition(bx);
    addAtlasItem("game-pics", sp);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Splash::decorate() {

  centerImage( "game.bg");
  regoAtlas("game-pics");

  // title
  auto cw = cx::center();
  auto wb = cx::visBox();
  addAtlasFrame(
      "game-pics", "title.png",
      c::Vec2(cw.x, wb.top * 0.9f));

  demo();

  auto b1= cx::reifyMenuBtn("play.png");
  auto f= []() { cx::prelude(); };
  auto menu= cx::mkMenu(b1);
  b1->setCallback(
      [=](c::Ref*) {
        cx::runEx(MMenu::reify(mc_new1(MCX, f)));
      });

  menu->setPosition( cw.x, wb.top * 0.1f);
  addItem(menu);
}



NS_END(tttoe)

