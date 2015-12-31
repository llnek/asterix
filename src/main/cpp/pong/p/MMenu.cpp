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
#include "x2d/XLib.h"
#include "NetPlay.h"
#include "MMenu.h"
#include "Game.h"
#include "n/GNodes.h"
#include "s/utils.h"
NS_ALIAS(ws, fusii::odin)
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL UILayer : public f::XLayer {

  STATIC_REIFY_LAYER(UILayer)

  virtual void decorate();

  virtual ~UILayer() {}
  UILayer() {}
  NOCPYASS(UILayer)

  void onPlayXXX(f::GMode, ws::OdinIO*, j::json);
  void onPlay3(c::Ref*);
  void onPlay2(c::Ref*);
  void onPlay1(c::Ref*);
  void onBack(c::Ref*);
  void onQuit(c::Ref*);

};

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::decorate() {
  auto tile = CC_CSV(c::Integer,"TILE");
  auto c = XCFG()->getColor("default");
  auto wb = cx::visBox();
  auto cw = cx::center();
  auto lb = cx::reifyBmfLabel(
      cw.x, wb.top * 0.9f,
      "font.JellyBelly", gets("mmenu"));

  centerImage("gui.mmenu.menu.bg");
  incIndexZ();

  lb->setScale(XCFG()->getScale());
  lb->setColor(c);
  addItem(lb);

  auto b1 = cx::reifyMenuBtn("online.png");
  b1->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onPlay3));

  auto b2 = cx::reifyMenuBtn("player2.png");
  b2->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onPlay2));

  auto b3 = cx::reifyMenuBtn("player1.png");
  b3->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onPlay1));

  auto menu= cx::mkVMenu(
      s_vec<c::MenuItem*> {b1,b2,b3});
  menu->setPosition(cw);
  addItem(menu);

  // back-quit button
  auto back= cx::reifyMenuBtn("icon_back.png");
  back->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onBack));
  back->setColor(c);

  auto quit= cx::reifyMenuBtn("icon_quit.png");
  quit->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onQuit));
  quit->setColor(c);

  auto m2= cx::mkHMenu(
      s_vec<c::MenuItem*> {back, quit} );
  auto sz= back->getContentSize();

  m2->setPosition(wb.left+tile+sz.width*1.1f,
                  wb.bottom+tile+sz.height*0.45f);
  addItem(m2);

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
void UILayer::onPlay3(c::Ref*) {
  // yes
  auto y= [=](ws::OdinIO* io, j::json obj) {
    this->onPlayXXX(f::GMode::NET, io, obj);
  };
  // no
  auto f= [=]() {
    cx::runScene(
        XCFG()->prelude(), this->getDelay());
  };
  auto n= [=]() {
    cx::runScene(
        MMenu::reify(
          mc_new_1(MCX,f)), this->getDelay());
  };

  cx::runScene(
      NetPlay::reify(
        mc_new_2(NPCX, y,n)), getDelay());
}

//////////////////////////////////////////////////////////////////////////
//
void UILayer::onPlay2(c::Ref *) {
  auto m=f::GMode::TWO;
  onPlayXXX(m, nullptr, fmtGameData(m));
}

//////////////////////////////////////////////////////////////////////////
//
void UILayer::onPlay1(c::Ref *) {
  auto m= f::GMode::ONE;
  onPlayXXX(m, nullptr, fmtGameData(m));
}

//////////////////////////////////////////////////////////////////////////
//
void UILayer::onPlayXXX(f::GMode mode, ws::OdinIO *io, j::json obj) {
  cx::runScene(
      Game::reify(
        mc_new_3(GCXX, mode, io, obj)), getDelay());
}

//////////////////////////////////////////////////////////////////////////
//
void UILayer::onBack(c::Ref*) {
  auto ctx = getSceneX()->getCtx();
  SCAST(MCX*,ctx)->back();
}

//////////////////////////////////////////////////////////////////////////
//
void UILayer::onQuit(c::Ref*) {
  cx::runScene(
      XCFG()->prelude(), getDelay());
}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
void MMenu::decorate() {
  UILayer::reify(this);
}


NS_END(pong)





