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

#include "core/Primitives.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "2d/CCMenuItem.h"
#include "2d/CCMenu.h"
#include "Menu.h"
#include "Game.h"
NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(invaders)

BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL UILayer : public f::XLayer {
protected:

  virtual f::XLayer* realize();
  void onPlay(c::Ref*);
  void onBack(c::Ref*);
  NO__CPYASS(UILayer)

public:

  IMPL_CTOR(UILayer)
};

//////////////////////////////////////////////////////////////////////////////
//
f::XLayer* UILayer::realize() {
  auto tile = CC_CSV(c::Integer, "TILE");
  auto color= c::Color3B(94,49,120);
  auto cw = cx::center();
  auto wb = cx::visBox();

  // background
  centerImage("mmenus.bg");

  // title
  auto lb= cx::reifyBmfLabel(cw.x, wb.top * 0.9f,
                          "font.JellyBelly",
                          XCFG()->getL10NStr("mmenu"));
  lb->setScale(XCFG()->getScale());
  lb->setColor(cx::white());
  addItem(lb);

  // play button
  auto b1= cx::reifyMenuBtn("play.png");
  b1->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onPlay));
  auto menu= cx::mkMenu(b1);
  menu->setPosition(cw);
  addItem(menu);

  // back-quit buttons
  auto b= cx::reifyMenuBtn("icon_back.png");
  b->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onBack));
  auto q= cx::reifyMenuBtn("icon_quit.png");
  q->setTarget(this,
      CC_MENU_SELECTOR(XLayer::onQuit));
  auto m2= cx::mkMenu(c::Vector<c::MenuItem*> {b, q}, false, 10.0f);
  auto sz= b->getContentSize();
  m2->setPosition(wb.left + tile + sz.width * 1.1f,
                  wb.bottom + tile + sz.height * 0.45f);
  addItem(m2);

  // audio
  c::MenuItem* off;
  c::MenuItem* on;
  cx::reifyAudioIcons(off, on);
  off->setColor(cx::white());
  on->setColor(cx::white());

  addAudioIcons(this, off, on,
      cx::anchorBR(),
      c::Vec2(wb.right - tile, wb.bottom + tile));

  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
void MenuLayer::OnPlay(c::Ref* r) {
  auto g = f::ReifyRefType<Game>();
    cx::RunScene(f::MainGame::Reify(g, f::GMode::ONE));
}

//////////////////////////////////////////////////////////////////////////////
//
void MenuLayer::OnBack(c::Ref* r) {
  CC_PCAST(MainMenu*)->OnBackAction();
}

//////////////////////////////////////////////////////////////////////////////
//
MainMenu* MainMenu::ReifyWithBackAction(c::CallFunc* back) {
  auto m = f::ReifyRefType<MainMenu>();
  m->backAction= back;
  back->retain();
  m->Realize();
  return m;
}

//////////////////////////////////////////////////////////////////////////////
//
void MainMenu::OnBackAction() {
  backAction->execute();
}

//////////////////////////////////////////////////////////////////////////////
//
f::XScene* MainMenu::Realize() {
  auto y = f::ReifyRefType<MenuLayer>();
  AddLayer(y);
  y->Realize();
  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
MainMenu::~MainMenu() {
  if (NNP(backAction)) { backAction->release(); }
}

//////////////////////////////////////////////////////////////////////////////
//
MainMenu::MainMenu() {
  SNPTR(backAction)
}



NS_END(invaders)

