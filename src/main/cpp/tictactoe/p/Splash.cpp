// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2015, Ken Leung. All rights reserved.

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "n/GNodes.h"
#include "s/utils.h"
#include "Menu.h"
#include "Splash.h"

NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
void Splash::demo() {
  auto scale= 0.75f;
  auto fm= "";
  auto ps= mapGridPos(scale);

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
    sp->setScale(scale);
    sp->setPosition(bx);
    addAtlasItem(this, "game-pics", sp);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Splash::decorate() {

  centerImage(this, "game.bg");
  incIndexZ();
  regoAtlas(this, "game-pics");

  // title
  auto cw = cx::center();
  auto wb = cx::visBox();
  addAtlasFrame(
      this, "game-pics", "title.png",
      c::Vec2(cw.x, wb.top * 0.9f));

  demo();

  auto f= [=]() { cx::runSceneEx(XCFG()->prelude()); };
  // play button
  auto b1= cx::reifyMenuBtn("play.png");
  auto menu= cx::mkMenu(b1);
  b1->setCallback([=](c::Ref*) {
    cx::runSceneEx( MMenu::reify(mc_new_1(MCX, f)));
      });

  menu->setPosition( cw.x, wb.top * 0.1f);
  addItem(this,menu);
}



NS_END(tttoe)

