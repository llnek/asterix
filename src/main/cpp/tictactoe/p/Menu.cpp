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
#include "core/odin.h"
#include "x2d/XLib.h"
#include "NetPlay.h"
#include "Menu.h"
#include "Game.h"
#include "n/GNodes.h"

NS_ALIAS(ws, fusii::odin)
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
void MMenu::decorate() {
  auto tile = CC_CSV(c::Integer,"TILE");
  auto nil = CC_CSV(c::Integer,"CV_Z");
  auto c = XCFG()->getColor("default");
  auto wb = cx::visBox();
  auto cw = cx::center();
  auto lb = cx::reifyBmfLabel(
      cw.x,
      wb.top * 0.9f,
      "font.JellyBelly",
      gets("mmenu"));

  centerImage(this,"gui.mmenu.menu.bg");
  incIndexZ();

  lb->setScale(XCFG()->getScale());
  lb->setColor(c);
  addItem(this,lb);

  auto b1 = cx::reifyMenuBtn("online.png");
  b1->setCallback([=](c::Ref*){
      this->onPlay3();
      });

  auto b2 = cx::reifyMenuBtn("player2.png");
  b2->setCallback([=](c::Ref*){
      this->onPlay2();
      });

  auto b3 = cx::reifyMenuBtn("player1.png");
  b3->setCallback([=](c::Ref*){
      this->onPlay1();
      });

  auto menu= cx::mkVMenu( s_vec<c::MenuItem*> {b1,b2,b3});
  menu->setPosition(cw);
  addItem(this,menu);

  // back-quit button
  auto back= cx::reifyMenuBtn("icon_back.png");
  back->setCallback([=](c::Ref*){
      this->onBack();
      });
  back->setColor(c);

  auto quit= cx::reifyMenuBtn("icon_quit.png");
  quit->setCallback([=](c::Ref*){
      this->onQuit();
      });
  quit->setColor(c);

  auto m2= cx::mkHMenu( s_vec<c::MenuItem*> {back, quit} );
  auto sz= back->getContentSize();

  m2->setPosition(wb.left+tile+sz.width*1.1f,
                  wb.bottom+tile+sz.height*0.45f);
  addItem(this,m2);

  // audio
  auto audios= cx::reifyAudioIcons();
  audios[0]->setColor(c);
  audios[1]->setColor(c);

  addAudioIcons(this, audios,
      cx::anchorBR(),
      c::Vec2(wb.right-tile, wb.bottom+tile));
}

//////////////////////////////////////////////////////////////////////////
//
void MMenu::onPlay3() {
  // yes
  auto y= [=](ws::OdinIO* io, j::json obj) {
    this->onPlayXXX(f::GMode::NET, io, obj);
  };
  // no
  auto f= []() {
    cx::runSceneEx(XCFG()->prelude());
  };
  auto n= [=]() {
    cx::runSceneEx(
        MMenu::reify(mc_new_1(MCX,f)));
  };

  cx::runSceneEx(
      NetPlay::reify( mc_new_2(NPCX, y,n)));
}

//////////////////////////////////////////////////////////////////////////
//
void MMenu::onPlay2() {
  auto m= f::GMode::TWO;
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
  cx::runSceneEx(
      Game::reify(mc_new_3(GCXX, mode, io, obj)));
}

//////////////////////////////////////////////////////////////////////////
//
void MMenu::onBack() {
  auto ctx = (MCX*) getCtx();
  ctx->back();
}

//////////////////////////////////////////////////////////////////////////
//
void MMenu::onQuit() {
  cx::runSceneEx( XCFG()->prelude());
}


NS_END(tttoe)


