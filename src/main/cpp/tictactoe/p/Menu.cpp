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
#include "x2d/Online.h"
#include "Menu.h"
#include "Game.h"
#include "n/cobjs.h"

NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
class CC_DLL MenuLayer : public f::XLayer {
public:
  virtual f::XLayer* Realize();
  void MaybeSeedGame(f::GMode);
  void OnPlayXXX(f::GMode);
  void OnNetPlay(c::Ref*);
  void OnPlay(c::Ref*);
  void OnBack(c::Ref*);
  NO__CPYASS(MenuLayer)
  IMPL_CTOR(MenuLayer)
};

//////////////////////////////////////////////////////////////////////////
//
f::XLayer* MenuLayer::Realize() {
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

  auto menu= f::ReifyRefType<cocos2d::Menu>();
  auto tile = CC_CSV(c::Integer,"TILE");
  auto nil = CC_CSV(c::Integer,"CV_Z");
  auto b= cx::ReifyMenuBtn("online.png");
  int tag = (int) f::GMode::NET;

  b->setTarget(this,
      CC_MENU_SELECTOR(MenuLayer::OnNetPlay));
  menu->addChild(b,0,tag);

  b= cx::ReifyMenuBtn("player2.png");
  b->setTarget(this,
      CC_MENU_SELECTOR(MenuLayer::OnPlay));
  tag= (int) f::GMode::TWO;
  menu->addChild(b,0,tag);

  b= cx::ReifyMenuBtn("player1.png");
  b->setTarget(this,
      CC_MENU_SELECTOR(MenuLayer::OnPlay));
  tag= (int) f::GMode:ONE;
  menu->addChild(b,0,tag);

  // add the menu
  menu->setPosition(cw);
  AddItem(menu);

  // back-quit button
  auto back= cx::ReifyMenuBtn("icon_back.png");
  back->setTarget(this,
      CC_MENU_SELECTOR(MenuLayer::OnBack));
  back->setColor(c::Color3B(c[0],c[1],c[2]));
  auto quit= cx::ReifyMenuBtn("icon_quit.png");
  quit->setTarget(this,
      CC_MENU_SELECTOR(XLayer::OnQuit));
  quit->setColor(c::Color3B(c[0],c[1],c[2]));

  auto m2= cx::MkBackQuit(back, quit, false);
  auto sz= back->getContentSize();

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

//////////////////////////////////////////////////////////////////////////////
//
void MenuLayer::MaybeSeedGame(f::GMode m) {

  j::Json seed = j::Json::object {
    {"ppids", j::Json::object {} }
  };

  switch (m) {
    case f::GMode::ONE:
      seed["ppids"][ XCFG()->GetL10NStr("p1") ] = j::Json::array {
        1, XCFG()->GetL10NStr("player1") };
      seed["ppids"][ XCFG()->GetL10NStr("p2") ] = j::Json::array {
        2, XCFG()->GetL10NStr("player2") };
    break;

    case f::GMode::TWO:
      seed["ppids"][ XCFG()->GetL10NStr("cpu") ] = j::Json::array {
        2, XCFG()->GetL10NStr("computer") };
      seed["ppids"][ XCFG()->GetL10NStr("p1") ] = j::Json::array {
        1, XCFG()->GetL10NStr("player1") };
    break;

    case f::GMode::NET:
    break;
  }

  XCFG()->SetSeedData(seed);
}

//////////////////////////////////////////////////////////////////////////
//
void MenuLayer::OnNetPlay(c::Ref* r) {
  // yes
  auto y= [=]() { this->OnPlayXXX(f::GMode::NET); };
  // no
  auto f= []() { cx::RunScene(XCFG()->StartWith()); };
  auto a= c::CallFunc::create(f);
  auto n= []() { cx::RunScene(MainMenu::ReifyWithBackAction(a)); };

  auto s= f::Online::Reify(f::ReifyRefType<NetPlay>(),
      c::CallFunc::create(y),
      c::CallFunc::create(n));
  cx::RunScene(s);
}

//////////////////////////////////////////////////////////////////////////
//
void MenuLayer::OnPlay(c::Ref* r) {
  auto mode = (f::GMode) SCAST(c::Node*, r)->getTag();
  OnPlayXXX(mode);
}

//////////////////////////////////////////////////////////////////////////
//
void MenuLayer::OnPlayXXX(f::GMode mode) {
  auto g = f::ReifyRefType<Game>();
  MaybeSeedGame(mode);
  cx::RunScene( Game::Reify(g, mode) );
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
  CC_DROP(backAction)
}

//////////////////////////////////////////////////////////////////////////////
//
MainMenu::MainMenu() {
  SNPTR(backAction)
}


NS_END(tttoe)

