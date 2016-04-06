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
#include "MMenu.h"
#include "Game.h"
#include "n/C.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
//
void MMenu::decoUI() {

  auto tt= cx::reifyBmfLabel("title", gets("mmenu"));
  auto c= XCFG()->getColor("text");
  auto ctx = (MCX*) getCtx();
  auto wb= cx::visBox();

  centerImage("game.bg");

  CC_POS2(tt, wb.cx, wb.top * 0.8);
  XCFG()->scaleLabel(tt,52);
  tt->setColor(c);
  addItem(tt);

  auto p= cx::reifyMenuBtn("player1.png");
  auto q= cx::reifyMenuBtn("quit.png");
  p->setCallback(
      [=](c::Ref*)
      { cx::runEx(
          Game::reify(mc_new(GameCtx))); });
  q->setCallback([=](c::Ref*) {
      cx::prelude();
      });
  auto menu=cx::mkVMenu(s_vec<c::MenuItem*>{p,q},
      CC_CHT(p) / GOLDEN_RATIO);
  CC_POS2(menu,wb.cx,wb.cy);
  addItem(menu);

  // back-quit button
  auto back= cx::reifyMenuBtn("icon_back.png");
  auto t= CC_CWH(back)/GOLDEN_RATIO;
  back->setAnchorPoint(cx::anchorBL());
  back->setCallback(
      [=](c::Ref*) { ctx->back(); });
  back->setColor(c);
  CC_POS2(back, wb.left + t, wb.bottom + t);
  addItem(cx::mkMenu(back));

  auto audios = cx::reifyAudioIcons();
  audios[0]->setColor(c);
  audios[1]->setColor(c);

  t= CC_CWH(audios[0])/GOLDEN_RATIO;
  addAudioIcons(audios,
    cx::anchorBR(),
    CCT_PT(wb.right - t, wb.bottom + t));
}



NS_END



