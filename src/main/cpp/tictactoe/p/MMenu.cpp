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
#include "core/Odin.h"
#include "NetPlay.h"
#include "MMenu.h"
#include "Game.h"
#include "n/lib.h"

NS_ALIAS(ws, fusii::odin)
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
void MMenu::decorate() {
  auto tile = CC_CSV(c::Integer,"TILE");
  auto c = XCFG()->getColor("dft");
  auto wb = cx::visBox();
  auto cw = cx::center();
  auto lb = cx::reifyBmfLabel(
      cw.x, wb.top * 0.9f,
      "font.JellyBelly", gets("mmenu"));

  centerImage("gui.mmenu.menu.bg");

  lb->setScale(XCFG()->getScale());
  lb->setColor(c);
  addItem(lb);

  //menu buttons
  auto b1 = cx::reifyMenuBtn("online.png");
  auto b2 = cx::reifyMenuBtn("player2.png");
  auto b3 = cx::reifyMenuBtn("player1.png");

  b1->setCallback(
      [=](c::Ref*){ this->onPlay3(); });

  b2->setCallback(
      [=](c::Ref*){ this->onPlayXXX(f::GMode::TWO ); } );

  b3->setCallback(
      [=](c::Ref*){ this->onPlayXXX(f::GMode::ONE ); } );

  //add btns to menu
  s_vec<c::MenuItem*> btns {b1,b2,b3};
  auto menu= cx::mkVMenu(btns);
  menu->setPosition(cw);
  addItem(menu);

  // back-quit button
  auto back= cx::reifyMenuBtn("icon_back.png");
  auto quit= cx::reifyMenuBtn("icon_quit.png");

  back->setColor(c);
  quit->setColor(c);

  back->setCallback(
      [=](c::Ref*){
      SCAST(MCX*, getCtx())->back(); });

  quit->setCallback(
      [=](c::Ref*){ cx::rxfs(); });

  // ctrl btns
  s_vec<c::MenuItem*> ctrl {back, quit};
  auto sz= back->getContentSize();
  auto m2= cx::mkHMenu(ctrl);

  m2->setPosition(wb.left+tile+sz.width*1.1f,
                  wb.bottom+tile+sz.height*0.45f);
  addItem(m2);

  // audio
  auto audios= cx::reifyAudioIcons();
  audios[0]->setColor(c);
  audios[1]->setColor(c);

  addAudioIcons(audios,
      cx::anchorBR(),
      c::Vec2(wb.right-tile, wb.bottom+tile));
}

//////////////////////////////////////////////////////////////////////////
//
void MMenu::onPlay3() {
  auto f = []() { cx::rxfs(); };
  auto x= new NPCX(

    [=](ws::OdinIO *io, j::json obj) {
      this->onPlayXXX(io, obj);
    },

    [=]() {
      cx::runEx(
          MMenu::reify(mc_new1(MCX,f))); }

  );

  cx::runEx(NetPlay::reify(x));
}

//////////////////////////////////////////////////////////////////////////
//
void MMenu::onPlayXXX(ws::OdinIO *io, j::json obj) {
  auto mode= f::GMode::NET;
  cx::runEx(
      Game::reify(
        mc_new3(GCXX, mode, io, obj)));
}

//////////////////////////////////////////////////////////////////////////
//
void MMenu::onPlayXXX(f::GMode mode) {
  auto obj = fmtGameData(mode);
  cx::runEx(
      Game::reify(
        mc_new2(GCXX, mode, obj)));
}


NS_END(tttoe)


