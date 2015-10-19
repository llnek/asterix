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

#include "core/CCSX.h"
#include "Menu.h"

NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
class CC_DLL MenuLayer : public f::XLayer {
public:
  void OnNetPlay(c::Ref*);
  void OnPlay(c::Ref*);
  void OnBack(c::Ref*);
  NO__CPYASS(MenuLayer)
  IMPL_CTOR(MenuLayer)
}

//////////////////////////////////////////////////////////////////////////
//
f::XLayer* MainMenuLayer::Realize() {
  //auto c= cx::ColorRGB("#f6b17f");
  auto c= cx::ColorRGB("#5E3178");
  auto wb=cx::VisBox();
  auto cw= cx::Center();
  auto lb= cx::ReifyBmfLabel(cw.x, wb.top * 0.9,
      "font.JellyBelly",
      XCFG()->GetL10NStr("mmenu"));

  CenterImage("gui.mmenu.menu.bg");
  IncIndexZ();

  lb->setColor(c::Color3B(c[0],c[1],c[2]));
  lb->setScale(XCFG()->GetScale());
  AddItem(lb);

  auto menu=  f::ReifyRefType<cocos2d::Menu>();
  auto b= cx::ReifyMenuBtn("online.png");
  auto seed= f::ReifyRefType<SeedData>();
  seed->mode = f::GMode::ONLINE;
  b->setUserObject(seed);
  b->setTarget(this,
      CC_MENU_SELECTOR(MenuLayer::OnNetPlay));
  menu->addChild(b);

  //p[ sh.l10n('%p1') ] = [ 1, sh.l10n('%player1') ];
  //p[ sh.l10n('%p2') ] = [ 2, sh.l10n('%player2') ];
  seed= f::ReifyRefType<SeedData>();
  seed->mode = f::GMode::TWO;
  b= cx::ReifyMenuBtn("player2.png");
  b->setUserObject(seed);
  b->setTarget(this,
      CC_MENU_SELECTOR(MenuLayer::OnPlay));
  menu->addChild(b);

  //p[ sh.l10n('%cpu') ] = [ 2, sh.l10n('%computer') ];
  //p[ sh.l10n('%p1') ] = [ 1,  sh.l10n('%player1') ];
  seed= f::ReifyRefType<SeedData>();
  seed->mode = f::GMode::ONE;
  b= cx::ReifyMenuBtn("player1.png");
  b->setUserObject(seed);
  b->setTarget(this,
      CC_MENU_SELECTOR(MenuLayer::OnPlay));
  menu->addChild(b);
  menu->setPosition(cw);
  AddItem(menu);

  // back-quit button
  auto b= cx::ReifyMenuBtn("icon_back.png");
  b->setTarget(this,
      CC_MENU_SELECTOR(MenuLayer::OnBack));
  b->setColor(c::Color3B(c[0],c[1],c[2]));
  auto q= cx::ReifyMenuBtn("icon_quit.png");
  q->setTarget(this,
      CC_MENU_SELECTOR(XLayer::OnQuit));
  q->setColor(c::Color3B(c[0],c[1],c[2]));
  auto m2= cx::MkBackQuit(b, q, false);
  auto sz= b->getContentSize();

  m2->setPosition(wb.left + tile + sz.width * 1.1,
                  wb.bottom + tile + sz.height * 0.45);
  AddItem(m2);

  // audio
  c::MenuItem* off;
  c::MenuItem* on;
  cx::ReifyAudioIcons(on,off);
  off->setColor(c::Color3B(c[0],c[1],c[2]));
  on->setColor(c::Color3B(c[0],c[1],c[2]));

  AddAudioIcons(off, on,
      cx::AnchorBR(),
      c::Vec2(wb.right - tile, wb.bottom + tile));

  return this;
}

//////////////////////////////////////////////////////////////////////////
//
void MenuLayer::OnNetPlay(c::Ref* r) {
  auto ol = f::ReifyRefType<Online>();
  auto y= []() { cx::RunScene(MainGame::Reify()); }
  auto yes= c::CallFunc::create(y);
  auto n= []() { cx::RunScene(MainMenu::Reify()); }
  auto no= c::CallFunc::create(n);
  auto s= Online::Reify(ol, yes,no);
  cx::RunScene(s);
}

//////////////////////////////////////////////////////////////////////////
//
void MenuLayer::OnPlay(c::Ref* r) {
  cx::RunScene( MainGame::Reify() );
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

NS_END(tttoe)
