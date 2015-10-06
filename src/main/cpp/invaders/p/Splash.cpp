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

#include "2d/CCActionInstant.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "x2d/XLayer.h"
#include "Menu.h"
#include "Splash.h"
NS_ALIAS(cx, fusii::ccsx)
NS_ALIAS(f, fusii)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL SplashLayer : public f::XLayer {
private:
  NO__CPYASS(SplashLayer)
  SplashLayer();

  void Title();
  void Btns();

public:

  virtual int GetIID() { return 1; }
  virtual f::XLayer* Realize();
  virtual ~SplashLayer();

  CREATE_FUNC(SplashLayer)
};

//////////////////////////////////////////////////////////////////////////////
//
SplashLayer::~SplashLayer() {
}

//////////////////////////////////////////////////////////////////////////////
//
SplashLayer::SplashLayer() {
}

//////////////////////////////////////////////////////////////////////////
//
f::XLayer* SplashLayer::Realize() {
  CenterImage("game.bg");
  Title();
  Btns();
  return this;
}

//////////////////////////////////////////////////////////////////////////
//
void SplashLayer::Title() {
  auto cw= cx::Center();
  auto wb= cx::VisBox();
  AddFrame("title.png", c::Vec2(cw.x, wb.top * 0.9));
}

//////////////////////////////////////////////////////////////////////////
//
void SplashLayer::Btns() {
  auto b1= cx::CreateMenuBtn("play.png");
  b1->setTarget(getParent(),
      CC_MENU_SELECTOR(Splash::OnPlay));
  auto menu= c::Menu::create();
  auto cw= cx::Center();
  auto wb= cx::VisBox();

  menu->addChild(b1);
  menu->setPosition( cw.x, wb.top * 0.1);
  AddItem(menu);
}

//////////////////////////////////////////////////////////////////////////
//
void Splash::OnPlay(c::Ref* b) {
  auto f= []() { cx::RunScene(XCFGS()->StartWith()); };
  auto a= c::CallFunc::create(f);
  auto m = MainMenu::CreateWithBackAction(a);

  cx::RunScene( m);
}

//////////////////////////////////////////////////////////////////////////////
//
f::XScene* Splash::Realize() {
  auto y = SplashLayer::create();
  AddLayer(y);
  y->Realize();
  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
Splash* Splash::Create() {
  auto s = Splash::create();
  s->Realize();
  return s;
}


NS_END(invaders)

