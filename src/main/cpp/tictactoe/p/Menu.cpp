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
#include "x2d/NetPlay.h"
#include "x2d/Funcs.h"
#include "Menu.h"
#include "Game.h"
#include "n/cobjs.h"
NS_BEGIN(tttoe)

BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
class CC_DLL MenuLayer : public f::XLayer {
public:
  virtual f::XLayer* realize();

  void maybeSeedGame(f::GMode);
  void onPlayXXX(f::GMode);
  void onNetPlay(c::Ref*);
  void onPlay(c::Ref*);
  void onBack(c::Ref*);
  void onQuit(c::Ref*);

  NO__CPYASS(MenuLayer)
  IMPL_CTOR(MenuLayer)
};

//////////////////////////////////////////////////////////////////////////////
//
f::XLayer* MenuLayer::realize() {
  //auto c= cx::ColorRGB("#f6b17f");
  auto c = cx::colorRGB("#5E3178");
  auto wb = cx::visBox();
  auto cw = cx::center();
  auto lb = cx::reifyBmfLabel(
      cw.x,
      wb.top * 0.9,
      "font.JellyBelly",
      XCFG()->getL10NStr("mmenu"));

  lb->setScale(XCFG()->getScale());
  lb->setColor(c);

  centerImage("gui.mmenu.menu.bg");
  incIndexZ();

  addItem(lb);

  auto menu = f::ReifyRefType<cocos2d::Menu>();
  auto tile = CC_CSV(c::Integer,"TILE");
  auto nil = CC_CSV(c::Integer,"CV_Z");
  auto b = cx::reifyMenuBtn("online.png");
  int tag = (int) f::GMode::NET;

  b->setTarget(this,
      CC_MENU_SELECTOR(MenuLayer::onNetPlay));
  menu->addChild(b,0,tag);

  b = cx::reifyMenuBtn("player2.png");
  tag = (int) f::GMode::TWO;
  b->setTarget(this,
      CC_MENU_SELECTOR(MenuLayer::onPlay));
  menu->addChild(b,0,tag);

  b = cx::reifyMenuBtn("player1.png");
  tag = (int) f::GMode::ONE;
  b->setTarget(this,
      CC_MENU_SELECTOR(MenuLayer::onPlay));
  menu->addChild(b,0,tag);

  // add the menu
  menu->setPosition(cw);
  addItem(menu);

  // back-quit button
  auto back= cx::reifyMenuBtn("icon_back.png");
  back->setTarget(this,
      CC_MENU_SELECTOR(MenuLayer::onBack));
  back->setColor(c);
  auto quit= cx::reifyMenuBtn("icon_quit.png");
  quit->setTarget(this,
      CC_MENU_SELECTOR(MenuLayer::onQuit));
  quit->setColor(c);

  auto m2= cx::mkMenu(s::vector<c::MenuItem*> {back, quit}, false, 10.0);
  auto sz= back->getContentSize();

  m2->setPosition(wb.left + tile + sz.width * 1.1,
                  wb.bottom + tile + sz.height * 0.45);
  addItem(m2);

  // audio
  c::MenuItem* off;
  c::MenuItem* on;
  cx::reifyAudioIcons(on,off);
  off->setColor(c);
  on->setColor(c);

  addAudioIcons((XLayer*) this,
      off, on,
      cx::anchorBR(),
      c::Vec2(wb.right - tile, wb.bottom + tile));

  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
void MenuLayer::maybeSeedGame(f::GMode m) {

  j::Json seed = j::Json::object {
    {"ppids", j::Json::object {} },
    {"pnum", 1 }
  };

  switch (m) {
    case f::GMode::ONE:
      seed["ppids"][ XCFG()->getL10NStr("p1") ] = j::Json::array {
        1, XCFG()->getL10NStr("player1") };
      seed["ppids"][ XCFG()->getL10NStr("p2") ] = j::Json::array {
        2, XCFG()->getL10NStr("player2") };
    break;

    case f::GMode::TWO:
      seed["ppids"][ XCFG()->getL10NStr("cpu") ] = j::Json::array {
        2, XCFG()->getL10NStr("computer") };
      seed["ppids"][ XCFG()->getL10NStr("p1") ] = j::Json::array {
        1, XCFG()->getL10NStr("player1") };
    break;

    case f::GMode::NET:
      seed["pnum"] = 0;
    break;
  }

  XCFG()->setSeedData(seed);
}

//////////////////////////////////////////////////////////////////////////
//
void MenuLayer::onNetPlay(c::Ref* r) {
  // yes
  auto y= [=]() { this->onPlayXXX(f::GMode::NET); };
  // no
  auto f= []() { cx::runScene(XCFG()->startWith()); };
  auto n= []() { cx::runScene(MainMenu::reifyWithBackAction(f)); };

  auto s= f::Online::reify(f::ReifyRefType<NetPlay>(), y,n);
  cx::runScene(s);
}

//////////////////////////////////////////////////////////////////////////
//
void MenuLayer::onPlay(c::Ref* r) {
  auto mode = (f::GMode) SCAST(c::Node*, r)->getTag();
  onPlayXXX(mode);
}

//////////////////////////////////////////////////////////////////////////
//
void MenuLayer::onPlayXXX(f::GMode mode) {
  auto g = f::ReifyRefType<Game>();
  maybeSeedGame(mode);
  cx::runScene( Game::reify(g, mode) );
}

//////////////////////////////////////////////////////////////////////////
//
void MenuLayer::onQuit(c::Ref* r) {

}

END_NS_UNAMED()

//////////////////////////////////////////////////////////////////////////////
//
MainMenu* MainMenu::reifyWithBackAction(VOIDFN cb) {
  auto m = f::ReifyRefType<MainMenu>();
  m->backAction= cb;
  m->realize();
  return m;
}

//////////////////////////////////////////////////////////////////////////////
//
void MainMenu::onBackAction() {
  this->backAction();
}

//////////////////////////////////////////////////////////////////////////////
//
f::XScene* MainMenu::realize() {
  auto y = f::ReifyRefType<MenuLayer>();
  addLayer(y);
  y->realize();
  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
MainMenu::~MainMenu() {

}

//////////////////////////////////////////////////////////////////////////////
//
MainMenu::MainMenu() {
  SNPTR(backAction)
}


NS_END(tttoe)

