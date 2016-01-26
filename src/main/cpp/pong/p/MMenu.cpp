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

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "core/Odin.h"
#include "NetPlay.h"
#include "MMenu.h"
#include "Game.h"
#include "n/N.h"
#include "n/lib.h"

NS_ALIAS(ws, fusii::odin)
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
void MMenu::decorate() {
  auto tile = CC_CSV(c::Float,"TILE");
  auto c = XCFG()->getColor("dft");
  auto wb = cx::visBox();
  auto lb = cx::reifyBmfLabel(
      wb.cx, wb.top * 0.9f,
      "JellyBelly", gets("mmenu"));

  centerImage("gui.mmenu.menu.bg");

  lb->setScale(XCFG()->getScale());
  lb->setColor(c);
  addItem(lb);

  auto b1 = cx::reifyMenuBtn("online.png");
  b1->setCallback(
      [=](c::Ref*) { this->onPlay3();  });

  auto b2 = cx::reifyMenuBtn("player2.png");
  b2->setCallback(
      [=](c::Ref*) { this->onPlay2();  });

  auto b3 = cx::reifyMenuBtn("player1.png");
  b3->setCallback(
      [=](c::Ref*) { this->onPlay1();  } );

  s_vec<c::MenuItem*> btns {b1,b2,b3};
  auto menu= cx::mkVMenu( btns);
  menu->setPosition(wb.cx, wb.cy);
  addItem(menu);

  // back-quit button
  auto back= cx::reifyMenuBtn("icon_back.png");
  auto quit= cx::reifyMenuBtn("icon_quit.png");
  s_vec<c::MenuItem*> bns {back, quit} ;
  auto ctx = getCtx();
  auto sz= back->getContentSize();
  auto m2= cx::mkHMenu(bns);

  quit->setColor(c);
  back->setColor(c);

  back->setCallback(
      [=](c::Ref*) { SCAST(MCX*,ctx)->back(); });

  quit->setCallback(
      [=](c::Ref*) { cx::prelude();  });

  m2->setPosition(wb.left+tile+sz.width*1.1f,
                  wb.bottom+tile+sz.height*0.45f);
  addItem(m2);

  // audio
  auto audios= cx::reifyAudioIcons();
  audios[0]->setColor(c);
  audios[1]->setColor(c);

  addAudioIcons( audios,
      cx::anchorBR(),
      c::Vec2(wb.right-tile, wb.bottom+tile));
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
void MMenu::onPlay2() {
  auto m=f::GMode::TWO;
  onPlayXXX(m, nullptr, fmtGameData(m));
}

//////////////////////////////////////////////////////////////////////////
//
void MMenu::onPlay1() {
  auto m= f::GMode::ONE;
  onPlayXXX(m, nullptr, fmtGameData(m));
}

//////////////////////////////////////////////////////////////////////////
//
void MMenu::onPlayXXX(f::GMode mode, ws::OdinIO *io, j::json obj) {
  cx::runEx(
      Game::reify(
        mc_new3(GCXX, mode, io, obj)));
}


NS_END(pong)





