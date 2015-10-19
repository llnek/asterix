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

NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL SplashLayer : public f::XLayer {
protected:
  void Demo();
public:
  virtual f::XLayer* Realize();
  NO__CPYASS(SplashLayer)
  IMPL_CTOR(SplashLayer)
}

//////////////////////////////////////////////////////////////////////////////
//
void SplashLayer::Demo() {
  auto scale= 0.75,
  auto fm= "";
  auto ps= MapGridPos(3,scale);

  // we scale down the icons to make it look nicer
  for (int i = 0; i < ps->Size(); ++i) {
    // set up the grid icons
    if (i == 1 || i == 5 || i == 6 || i == 7) { fm= "x.png"; }
    else if (i == 0 || i == 4) { fm= "z.png"; }
    else { fm= "o.png"; }
    auto sp= new c::Sprite(fm);
    bx=cx::VBoxMID( ps->Get(i));
    sp->setScale(scale);
    sp->setPosition(bx.x,bx.y);
    AddAtlasItem("game-pics",sp);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
f::XLayer* SplashLayer::Realize() {

  CenterImage("game.bg");
  IncIndexZ();
  RegoAtlas("game-pics");

  // title
  auto cw = cx::Center();
  auto wb = cx::VisBox();
  AddAtlasFrame("game-pics", "title.png",
                     c::Vec2(cw.x, wb.top * 0.9));

  Demo();

  // play button
  auto menu=  f::ReifyRefType<cocos2d::Menu>();
  auto b1= cx::ReifyMenuBtn("play.png");
  b1->setTarget(getParent(),
      CC_MENU_SELECTOR(Splash::OnPlay));
  menu->addChild(b1);
  menu->setPosition( cw.x, wb.top * 0.1);
  AddItem(menu);

  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
f::XScene* Splash::Realize() {
  auto y = f::ReifyRefType<SplashLayer>();
  AddLayer(y);
  y->Realize();
  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
void Splash::OnPlay(c::Ref* rr) {

  auto f= []() { cx::RunScene(XCFG()->StartWith()); };
  auto a= c::CallFunc::create(f);
  auto m = MainMenu::ReifyWithBackAction(a);

  cx::RunScene( m);
}




NS_END(tttoe)
#endif
