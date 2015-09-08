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

#include "Menu.h"
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////////
//
void MenuLayer::Title() {
  auto cfg = Config::GetInstance();
  auto wb= CCSX::VisBox();
  auto cw= CCSX::Center();
  auto lb= CreateBMFLabel(cw.x, wb.top * 0.9,
                          "font.JellyBelly",
                          "some text");
  lb->setScale(cfg->GetScale());
  lb->setColor(CCSX::WHITE);
  AddItem(lb);
}

//////////////////////////////////////////////////////////////////////////////
//
void MenuLayer::OnInit() {
  auto cfg = Config::GetInstance();
  auto tile = SCAST(Integer*, cfg->GetCst("TILE"));
  auto img = cfg->GetImage("gui.mmenus.menu.bg");
  CenterImage(img);
  Title();
  auto color= Color3B(94,49,120);
  auto cw = CCSX::Center();
  auto wb = CCSX::VisBox();

  // play button
  auto b1= CreateMenuBtn("#play.png",
      "#play.png", "#play.png",
      CC_CALLBACK_1(MenuLayer::OnPlay, this));
  auto menu= Menu::create();
  menu->addChild(b1);
  menu->setPosition(cw);
  AddItem(menu);

  // back-quit button
  auto b= CreateMenuBtn("#icon_back.png",
      "#icon_back.png",
      "#icon_back.png",
      CC_CALLBACK_1(MenuLayer::OnBack, this));
  auto q= CreateMenuBtn("#icon_quit.png",
      "#icon_quit.png",
      "#icon_quit.png",
      CC_CALLBACK_1(MenuLayer::OnQuit, this));
  auto m2= MkBackQuit(false, b, q);
  auto sz= b->getContentSize();
  auto gap = tile->getValue();
  m2->setPosition(wb.left + gap + sz.width * 1.1,
                  wb.bottom + gap + sz.height * 0.45);
  AddItem(m2);

  MenuItem* off;
  MenuItem* on;
  CreateAudioIcons(on,off);
  off->setColor(CCSX::WHITE);
  on->setColor(CCSX::WHITE);
  AddAudioIcons(off, on, Anchor::BottomRight,
      Vec2(wb.right - gap, wb.bottom + gap));
}

//////////////////////////////////////////////////////////////////////////////
//
void MenuLayer::OnPlay() {
  auto g= MainGame::create();
  CCSX::RunScene(g);
}

//////////////////////////////////////////////////////////////////////////////
//
MenuLayer::OnBack(Ref* r) {
  auto p = SCAST(Menu*, getParent());
  p->OnBackAction();
}

//////////////////////////////////////////////////////////////////////////////
//
MenuLayer::OnQuit(Ref* r) {
  auto p = SCAST(Menu*, getParent());
  p->OnQuitAction();
}

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
MainMenu* MainMenu::CreateWithBackAction(FiniteTimeAction* back) {
  backAction= back;
  back->retain();
}

//////////////////////////////////////////////////////////////////////////////
//
void MainMenu::OnBackAction(Ref* s) {
  backAction->execute();
}

//////////////////////////////////////////////////////////////////////////////
//
void MainMenu::OnInit() {
  auto m = MenuLayer::create();
  addChild(m);
}

//////////////////////////////////////////////////////////////////////////////
//
MainMenu::~MainMenu() {
  backAction->release();
}

//////////////////////////////////////////////////////////////////////////////
//
MainMenu::MainMenu()
  : backAction(nullptr) {
}





NS_END(invaders)

