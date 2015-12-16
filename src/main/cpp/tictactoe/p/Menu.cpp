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
#include "x2d/XLib.h"
#include "NetPlay.h"
#include "Menu.h"
#include "Game.h"
#include "n/GNodes.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tttoe)


BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
class CC_DLL UILayer : public f::XLayer {
protected:

  void onPlayXXX(f::GMode);
  void onPlay3(c::Ref*);
  void onPlay2(c::Ref*);
  void onPlay1(c::Ref*);
  void onBack(c::Ref*);
  void onQuit(c::Ref*);

public:

  STATIC_REIFY_LAYER(UILayer)

  virtual void decorate();

  virtual ~UILayer() {}
  UILayer() {}

  NOCPYASS(UILayer)
};

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::decorate() {
  //auto c= cx::colorRGB("#f6b17f");
  auto c = cx::colorRGB("#5E3178");
  auto wb = cx::visBox();
  auto cw = cx::center();
  auto lb = cx::reifyBmfLabel(
      cw.x,
      wb.top * 0.9f,
      "font.JellyBelly",
      XCFG()->getL10NStr("mmenu"));

  lb->setScale(XCFG()->getScale());
  lb->setColor(c);

  centerImage("gui.mmenu.menu.bg");
  //incIndexZ();

  addItem(lb);

  auto tile = CC_CSV(c::Integer,"TILE");
  auto nil = CC_CSV(c::Integer,"CV_Z");

  auto b1 = cx::reifyMenuBtn("online.png");
  b1->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onPlay3));

  auto b2 = cx::reifyMenuBtn("player2.png");
  b2->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onPlay2));

  auto b3 = cx::reifyMenuBtn("player1.png");
  b3->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onPlay1));

  auto menu= cx::mkVMenu(s_vec<c::MenuItem*> {b1,b2,b3});
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

  auto m2= cx::mkHMenu(s_vec<c::MenuItem*> {back, quit} );
  auto sz= back->getContentSize();

  m2->setPosition(wb.left + tile + sz.width * 1.1f,
                  wb.bottom + tile + sz.height * 0.45f);
  addItem(m2);

  // audio
  auto audios= cx::reifyAudioIcons(on, off);
  audios[0]->setColor(c);
  audios[1]->setColor(c);

  addAudioIcons((XLayer*) this, audios,
      cx::anchorBR(),
      c::Vec2(wb.right - tile, wb.bottom + tile));
}

//////////////////////////////////////////////////////////////////////////
//
void UILayer::onPlay3(c::Ref *r) {
  // yes
  auto y= [=]() { this->onPlayXXX(f::GMode::NET); };
  // no
  auto f= [=]() { cx::runScene(XCFG()->prelude()); };
  auto n= [=]() { cx::runScene(MMenu::reify(mc_new_1(MCX,f))); }

  auto p= NetPlay::reify( mc_new_2(NPCX, y,n));
  cx::runScene(p);
}

//////////////////////////////////////////////////////////////////////////
//
void UILayer::onPlay2(c::Ref *) {
  onPlayXXX(f::GMode::TWO);
}
//////////////////////////////////////////////////////////////////////////
//
void UILayer::onPlay1(c::Ref *) {
  onPlayXXX(f::GMode::ONE);
}

//////////////////////////////////////////////////////////////////////////
//
void UILayer::onPlayXXX(f::GMode mode) {
  auto g = Game::reify(mc_new(GCXX));
  cx::runScene(g);
}

//////////////////////////////////////////////////////////////////////////
//
void UILayer::onBack(c::Ref*) {
  auto ctx = (MCX*) MGMS()->getCtx();
  ctx()->back();
}

//////////////////////////////////////////////////////////////////////////
//
void UILayer::onQuit(c::Ref*) {
  cx::runScene(XCFG()->prelude());
}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
void MMenu::decorate() {
  UILayer::reify(this);
}



NS_END(tttoe)


