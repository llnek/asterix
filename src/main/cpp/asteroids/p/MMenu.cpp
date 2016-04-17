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
#include "MMenu.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(asteroids)

//////////////////////////////////////////////////////////////////////////////
//
void MMenu::decoUI() {

  auto tt= cx::reifyBmfLabel("title", gets("mmenu"));
  XCFG()->scaleBmfont(tt, 64);
  auto c= XCFG()->getColor("dft");
  auto wb=cx::visBox();

  CC_POS2(tt, wb.cx, wb.top * 0.8);
  tt->setColor(c);

  centerImage("game.bg");
  addItem(tt);

  auto r= cx::reifyMenuText("btns", "Restart");
  auto q= cx::reifyMenuText("btns", "Quit");
  XCFG()->scaleBmfont(r, 36);
  XCFG()->scaleBmfont(q, 36);
  r->setCallback([=](c::Ref*){
      cx::runEx(Game::reify(mc_new(GameCtx))); });
  q->setCallback([=](c::Ref*){ cx::prelude(); });
  auto menu= cx::mkVMenu(s_vec<c::MenuItem*> {r,q},
      CC_CHT(r)/GOLDEN_RATIO);
  CC_POS2(menu, wb.cx, wb.top);
  addItem(menu);

  // back-quit button
  auto back= cx::reifyMenuBtn("icon_back.png");
  XCFG()->fit(back);
  back->setAnchorPoint(cx::anchorBL());
  auto ctx= (MCX*) getCtx();
  auto gap= CC_CHT(back)/GOLDEN_RATIO;
  back->setColor(c);
  back->setCallback([=](c::Ref*) { ctx->back(); });
  CC_POS2(back, wb.left + gap,
                wb.bottom + gap);
  addItem(cx::mkMenu(back));

  // audio btns
  auto audios= cx::reifyAudioIcons();
  audios[0]->setColor(c);
  audios[1]->setColor(c);
  gap = CC_CHT(audios[0])/GOLDEN_RATIO;
  addAudioIcons( audios,
    cx::anchorBR(),
    CCT_PT(wb.right - gap, wb.bottom + gap));

}


NS_END

