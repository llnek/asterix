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
#include "core/Odin.h"
#include "NetPlay.h"
#include "MMenu.h"
#include "Game.h"
#include "n/C.h"

NS_ALIAS(ws, fusii::odin)
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
void MMenu::decoUI() {
  auto c = XCFG()->getColor("dft");
  auto wb = cx::visBox();
  auto lb = cx::reifyBmfLabel(
      wb.cx, wb.top * 0.8,
      "title", gets("mmenu"));

  centerImage("game.bg");
  regoAtlas("cc-pics");

  XCFG()->scaleBmfont(lb, 52);
  lb->setColor(c);
  addItem(lb);

  auto b1 = cx::reifyMenuText("btns", "Online");
  b1->setColor(c::Color3B::WHITE);
  XCFG()->scaleBmfont(b1,36);
  b1->setCallback(
      [=](c::Ref*) { this->onPlay3();  });

  auto b2 = cx::reifyMenuText("btns", "Play");
  b2->setColor(c::Color3B::WHITE);
  XCFG()->scaleBmfont(b2,36);
  b2->setCallback(
      [=](c::Ref*) { this->onPlay1();  } );

  auto b3 = cx::reifyMenuText("btns", "Quit");
  b3->setColor(c::Color3B::WHITE);
  XCFG()->scaleBmfont(b3,36);
  b3->setCallback(
      [=](c::Ref*) { cx::prelude();  } );

  auto menu= cx::mkVMenu(s_vec<c::MenuItem*> {b1,b2,b3},
      CC_CHT(b1)/GOLDEN_RATIO);
  CC_POS2(menu, wb.cx, wb.cy);
  addItem(menu);

  // back-quit button
  auto back= cx::reifyMenuBtn("icon_back.png");
  auto sz= CC_CSIZE(back);
  auto gap= sz.width/GOLDEN_RATIO;
  auto ctx = getCtx();

  back->setCallback(
      [=](c::Ref*) { SCAST(MCX*,ctx)->back(); });
  back->setColor(c);

  back->setAnchorPoint(cx::anchorBL());
  CC_POS2(back,wb.left+gap,
               wb.bottom+gap);
  addItem(cx::mkMenu(back));

  // audio
  auto audios= cx::reifyAudioIcons();
  sz=CC_CSIZE(audios[0]);
  gap=sz.width/GOLDEN_RATIO;
  audios[0]->setColor(c);
  audios[1]->setColor(c);

  addAudioIcons(audios,
      cx::anchorBR(),
      CCT_PT(wb.right-gap, wb.bottom+gap));
}

//////////////////////////////////////////////////////////////////////////
//
void MMenu::onPlay3() {
  // no
  auto f= []() { cx::prelude(); };
  auto n= [=]() { cx::runEx( MMenu::reify( mc_new1(MCX,f))); };
  // yes
  auto y= [=](ws::OdinIO* io, j::json obj) {
    this->onPlayXXX(f::GMode::NET, io, obj);
  };

  cx::runEx( NetPlay::reify( mc_new2(NPCX, y,n)));
}

//////////////////////////////////////////////////////////////////////////
//
void MMenu::onPlay1() {
  auto m= f::GMode::ONE;
  onPlayXXX(m, CC_NIL, fmtGameData(m));
}

//////////////////////////////////////////////////////////////////////////
//
void MMenu::onPlayXXX(f::GMode mode, ws::OdinIO *io, j::json obj) {
  cx::runEx(
      Game::reify(
        mc_new3(GCXX, mode, io, obj)));
}



NS_END





