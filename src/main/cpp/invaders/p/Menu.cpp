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


//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL MenuLayer : public f::XLayer {
private:
  NO__CPYASS(MenuLayer)
  MenuLayer();
  void Title();

public:
  virtual XLayer* Realize();

  virtual int GetIID() { return 1; }
  virtual ~MenuLayer();

  void OnPlay(c::Ref*);
  void OnBack(c::Ref*);

  // standard create
  CREATE_FUNC(MenuLayer)
};

//////////////////////////////////////////////////////////////////////////////
//
MenuLayer::~MenuLayer() {
}

//////////////////////////////////////////////////////////////////////////////
//
MenuLayer::MenuLayer() {
}

//////////////////////////////////////////////////////////////////////////////
//
f::XLayer* MenuLayer::Realize() {
  auto tile = f::CstVal<c::Integer>("TILE")->getValue();
  auto color= c::Color3B(94,49,120);
  auto cw = cx::Center();
  auto wb = cx::VisBox();

  // background
  CenterImage("mmenus.bg");

  // title
  auto lb= cx::ReifyBmfLabel(cw.x, wb.top * 0.9,
                          "font.JellyBelly",
                          "some text");
  lb->setScale(XCFG()->GetScale());
  lb->setColor(cx::White());
  AddItem(lb);

  // play button
  auto b1= cx::ReifyMenuBtn("play.png");
  b1->setTarget(this,
      CC_MENU_SELECTOR(MenuLayer::OnPlay));
  auto menu= c::Menu::create();
  menu->addChild(b1);
  menu->setPosition(cw);
  AddItem(menu);

  // back-quit button
  auto b= cx::ReifyMenuBtn("icon_back.png");
  b->setTarget(this,
      CC_MENU_SELECTOR(MenuLayer::OnBack));
  auto q= cx::ReifyMenuBtn("icon_quit.png");
  q->setTarget(this,
      CC_MENU_SELECTOR(XLayer::OnQuit));
  auto m2= cx::MkBackQuit(b, q, false);
  auto sz= b->getContentSize();

  m2->setPosition(wb.left + tile + sz.width * 1.1,
                  wb.bottom + tile + sz.height * 0.45);
  AddItem(m2);

  // audio
  c::MenuItem* off;
  c::MenuItem* on;
  cx::ReifyAudioIcons(on,off);
  off->setColor(cx::White());
  on->setColor(cx::White());

  AddAudioIcons(off, on,
      cx::AnchorBR(),
      c::Vec2(wb.right - tile, wb.bottom + tile));

  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
void MenuLayer::OnPlay(c::Ref* r) {
  cx::RunScene(Game::Reify());
}

//////////////////////////////////////////////////////////////////////////////
//
void MenuLayer::OnBack(c::Ref* r) {
  auto p = (MainMenu*) getParent();
  p->OnBackAction();
}

//////////////////////////////////////////////////////////////////////////////
//
MainMenu* MainMenu::ReifyWithBackAction(c::CallFunc* back) {
  auto m = new MainMenu();
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
  auto y = MenuLayer::create();
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

