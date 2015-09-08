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
NS_BEGIN(fusilli)


//////////////////////////////////////////////////////////////////////////////
//
void MenuLayer::Title() {
  auto wb= CCSX::VisBox();
  auto cw= CCSX::Center();
  auto lb= CreateBMFLabel(cw.x, wb.top * 0.9, "font.JellyBelly", "some text");
  lb->setColor(CCSX::WHITE);
  lb->setScale(xcfg.game.scale);
  AddItem(lb);
}

//////////////////////////////////////////////////////////////////////////////
//
void MenuLayer::Setup() {
  auto cfg = Config::GetInstance();
  auto img = cfg->GetImage("gui.mmenus.menu.bg");
  CenterImage(img);
  Title();
  auto color= Color3B(94,49,120);
  auto cw = CCSX::Center();
  auto wb = CCSX::VisBox();
  auto b1= CreateMenuBtn("#play.png",
      "#play.png", "#play.png",
      CC_CALLBACK_1(MenuLayer::OnPlay, this));
  auto menu= Menu::create();
  menu->addChild(b1);
  menu->setPosition(cw);
  AddItem(menu);

  auto b= CreateMenuBtn("#icon_back.png",
      "#icon_back.png",
      "#icon_back.png",
      CC_CALLBACK_1(Menu::OnBack,this));
  auto q= CreateMenuBtn("#icon_quit.png",
      "#icon_quit.png",
      "#icon_quit.png",
      CC_CALLBACK_1(Menu::OnQuit,this));
  auto m2= MkBackQuit(false, b, q);
  auto sz= b->getContentSize();
  m2->setPosition(wb.left + csts.TILE + sz.width * 1.1,
                  wb.bottom + csts.TILE + sz.height * 0.45);
  AddItem(m2);

  MenuItem* off;
  MenuItem* on;
  CreateAudioIcons(on,off);
  off->setColor(CCSX::WHITE);
  on->setColor(CCSX::WHITE);
  AddAudioIcons(off, on, Anchor::BottomRight,
      Vec2(wb.right - csts.TILE, wb.bottom + csts.TILE));
}

void MenuLayer::OnPlay() {
  auto g= MainGame::create();
  g.Setup();
  CCSX::RunScene(g);
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
void MainMenu::CreateLayers() {
  auto m = MenuLayer::create();
  addChild(m);
}


//////////////////////////////////////////////////////////////////////////////
//
MainMenu::~MainMenu() {
}


//////////////////////////////////////////////////////////////////////////////
//
MainMenu::MainMenu() {
}





NS_END(fusilli)

