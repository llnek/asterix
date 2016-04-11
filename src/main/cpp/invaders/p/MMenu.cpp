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

#include "2d/CCMenuItem.h"
#include "2d/CCMenu.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "MMenu.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////////
//
void MMenu::decoUI() {

  auto dfc= XCFG()->getColor("dft");
  auto ctx = (MCX*) getCtx();
  auto wb = cx::visBox();

  centerImage("game.bg");
  regoAtlas("cc-pics");

  // title
  auto lb= cx::reifyBmfLabel(wb.cx, wb.top * 0.8,
      "title",
      gets("mmenu"));
  lb->setColor(XCFG()->getColor("text"));
  XCFG()->scaleNode(lb, 52);
  addItem(lb);

  // play button
  auto b1= cx::reifyMenuText("btns", "Restart");
  auto b2= cx::reifyMenuText("btns", "Quit");
  auto menu= cx::mkVMenu(s_vec<c::MenuItem*>{b1,b2},
      CC_CHT(b1)/GOLDEN_RATIO);

  b2->setCallback([=](c::Ref*) { cx::prelude(); });
  b1->setCallback([=](c::Ref*)
      { cx::runEx(Game::reify(mc_new(GameCtx))); });
  CC_POS2(menu, wb.cx, wb.cy);
  addItem(menu);

  auto b= cx::reifyMenuBtn("icon_back.png");
  auto gap= CC_CWH(b)/GOLDEN_RATIO;

  b->setCallback([=](c::Ref*) { ctx->back(); });
  b->setAnchorPoint(cx::anchorBL());
  CC_POS2(b, wb.left + gap,
             wb.bottom + gap);
  addItem(cx::mkMenu(b));

  auto audios = cx::reifyAudioIcons();
  audios[0]->setColor(dfc);
  audios[1]->setColor(dfc);

  gap= CC_CWH(audios[0])/GOLDEN_RATIO;
  addAudioIcons(audios,
      cx::anchorBR(),
      CCT_PT(wb.right - gap, wb.bottom + gap));
}


NS_END

