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
#include "Game.h"
#include "Ende.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////////
//
void Ende::decoUI() {

  auto qn= cx::reifyBmfLabel("title", gets("gameover"));
  auto wb= cx::visBox();

  centerImage("game.bg")->setOpacity(0.8 * 255);

  qn->setColor(XCFG()->getColor("text"));
  CC_POS2(qn, wb.cx, wb.top * 0.8);
  XCFG()->scaleNode(qn,52);
  addItem(qn);

  // btns
  auto b1= cx::reifyMenuText("btns", "TryAgain?");
  auto b2= cx::reifyMenuText("btns", "Quit");
  XCFG()->scaleNode(b2,36);
  XCFG()->scaleNode(b1,36);
  s_vec<c::MenuItem*> btns {b1, b2};
  auto menu= cx::mkVMenu(s_vec<c::MenuItem*> {b1,b2},
      CC_CHT(b1)/GOLDEN_RATIO);

  b1->setCallback([=](c::Ref*)
      { cx::runEx(Game::reify(mc_new(GameCtx))); });
  b2->setCallback([](c::Ref*) { cx::prelude(); });

  CC_POS2(menu, wb.cx, wb.cy);
  addItem(menu);
}


NS_END


