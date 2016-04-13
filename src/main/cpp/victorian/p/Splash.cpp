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
#include "Splash.h"
#include "Game.h"
#include "n/C.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(victorian)

//////////////////////////////////////////////////////////////////////////////
//
void Splash::decoUI() {

  auto how= cx::reifyMenuBtn("btn_howto_on.png", "btn_howto_off.png");
  auto play= cx::reifyMenuBtn("btn_new_on.png", "btn_new_off.png");
  auto ctx= mc_new1(GameCtx,kGamePlay);
  play->setCallback([=](c::Ref*) {
      cx::runEx(Game::reify(ctx));
  });

  ctx= mc_new1(GameCtx,kGameTutorial);
  how->setCallback([=](c::Ref*) {
        cx::runEx(Game::reify(ctx));
  });

  auto menu= cx::mkHMenu(s_vec<c::MenuItem*> {how,play});
  auto wb=cx::visBox();

  centerImage("game.bg");

  menu->setPosition(wb.cx, wb.top * 0.54);
  addItem(menu, kForeground);

  auto intro = cx::reifySprite("logo.png");
  intro->setPosition(wb.cx, wb.top * 0.7);
  addItem(intro, kForeground);

}



NS_END


