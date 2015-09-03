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

#include "Splash.h"
NS_FI_BEGIN

//////////////////////////////////////////////////////////////////////////
//
void SplashLayer::Setup() {
  CenterImage("game.bg");
  Title();
  Btns();
}

void SplashLayer::OnPlay(Node* b) {

  CCSX::RunScene( MainMenu::create());
  /*
  mm.reify({
    onback() { ccsx.runScene( ss.reify() ); }
  }));*/

}

void SplashLayer::Title() {
  auto cw= CCSX::Center();
  auto wb= CCSX::VisBox();
  AddFrame("#title.png", Vec2(cw.x, wb.top * 0.9));
}

void SplashLayer::Btns() {
  auto cw= CCSX::Center();
  auto wb= CCSX::VisBox();
  auto b1= CreateMenuBtn("#play.png",
      "#play.png", "#play.png",
      CC_CALLBACK_1(SplashLayer::OnPlay, this));
  auto menu= Menu::create();
  menu->addChild(b1);
  menu->setPosition( cw.x, wb.top * 0.1);
  AddItem(menu);
}


void Splash::Setup() {

  auto s= SplashLayer::create();
  s.Setup();
  addChild(s);

}

NS_FI_END

