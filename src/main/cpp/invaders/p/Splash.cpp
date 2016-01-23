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

#include "2d/CCActionInstant.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "x2d/XLayer.h"
#include "MMenu.h"
#include "Splash.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////
//
void Splash::decorate() {

  auto b1 = cx::reifyMenuBtn("play.png");
  auto f= []() { cx::prelude(); };
  auto menu = cx::mkMenu(b1);
  auto wb = cx::visBox();
  auto x = mc_new1(MCX, f);

  centerImage("game.bg");

  addFrame( "title.png", c::Vec2(wb.cx, wb.top * 0.9f));

  b1->setCallback(
      [=](c::Ref*) { cx::runEx(MMenu::reify(x)); });

  menu->setPosition( wb.cx, wb.top * 0.1f);

  addItem(menu);
}

NS_END(invaders)

