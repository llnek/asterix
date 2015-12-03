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
#include "n/CObjs.h"
NS_BEGIN(tttoe)

BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
class CC_DLL UILayer : public f::XLayer {
public:
  virtual f::XLayer* realize();

  void onPlayXXX(f::GMode);
  void onNetPlay(c::Ref*);
  void onPlay(c::Ref*);
  void onBack(c::Ref*);
  void onQuit(c::Ref*);

  NOCPYASS(UILayer)
  IMPLCZ(UILayer)
};

//////////////////////////////////////////////////////////////////////////////
//
f::XLayer* UILayer::realize() {
  //auto c= cx::colorRGB("#f6b17f");
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
  //incIndexZ();

  addItem(lb);

  auto menu = f::reifyRefType<cocos2d::Menu>();
  auto tile = CC_CSV(c::Integer,"TILE");
  auto nil = CC_CSV(c::Integer,"CV_Z");
  int tag = (int) f::GMode::NET;
  auto b = cx::reifyMenuBtn("online.png");
  b->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onNetPlay));
  menu->addChild(b,0,tag);

  b = cx::reifyMenuBtn("player2.png");
  tag = (int) f::GMode::TWO;
  b->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onPlay));
  menu->addChild(b,0,tag);

  b = cx::reifyMenuBtn("player1.png");
  tag = (int) f::GMode::ONE;
  b->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onPlay));
  menu->addChild(b,0,tag);

  // add the menu
  menu->alignItemsVerticallyWithPadding(10.0f);
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

  auto m2= cx::mkMenu(s::vector<c::MenuItem*> {back, quit}, false, 10.0);
  auto sz= back->getContentSize();

  m2->setPosition(wb.left + tile + sz.width * 1.1,
                  wb.bottom + tile + sz.height * 0.45);
  addItem(m2);

  // audio
  c::MenuItem* off;
  c::MenuItem* on;
  cx::reifyAudioIcons(on, off);
  off->setColor(c);
  on->setColor(c);

  addAudioIcons((XLayer*) this,
      off, on,
      cx::anchorBR(),
      c::Vec2(wb.right - tile, wb.bottom + tile));

  return this;
}

//////////////////////////////////////////////////////////////////////////
//
void UILayer::onNetPlay(c::Ref* r) {
  // yes
  auto y= [=]() { this->onPlayXXX(f::GMode::NET); };
  // no
  auto f= [=]() { cx::runScene(XCFG()->startWith()); };
  auto n= [=]() { cx::runScene(MainMenu::reifyWithBackAction(f)); };

  auto s= f::reifyRefType<NetPlay>();
  cx::runScene(s);
}

//////////////////////////////////////////////////////////////////////////
//
void UILayer::onPlay(c::Ref* r) {
  auto mode = (f::GMode) SCAST(c::Node*, r)->getTag();
  onPlayXXX(mode);
}

//////////////////////////////////////////////////////////////////////////
//
void UILayer::onPlayXXX(f::GMode mode) {
  auto g = f::reifyRefType<Game>();
  prepareSeedData(mode);
  cx::runScene( Game::reify(g, mode) );
}

//////////////////////////////////////////////////////////////////////////
//
void UILayer::onBack(c::Ref* r) {
  auto p= (MainMenu*) getParent();
  p->backAction();
}

//////////////////////////////////////////////////////////////////////////
//
void UILayer::onQuit(c::Ref* r) {
  cx::runScene(XCFG()->startWith());
}

END_NS_UNAMED()

//////////////////////////////////////////////////////////////////////////////
//
MainMenu* MainMenu::reifyWithBackAction(VOIDFN cb) {
  auto m = f::reifyRefType<MainMenu>();
  m->backAction= cb;
  m->realize();
  return m;
}

//////////////////////////////////////////////////////////////////////////////
//
f::XScene* MainMenu::realize() {
  auto y = f::reifyRefType<UILayer>();
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

