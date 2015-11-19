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
#include "n/cobjs.h"
#include "Splash.h"
NS_BEGIN(tttoe)

BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL SplashLayer : public f::XLayer {
public:
  virtual f::XLayer* realize();

  void onPlay(c::Ref*);
  void demo();

  NO__CPYASS(SplashLayer)
  IMPL_CTOR(SplashLayer)
};

//////////////////////////////////////////////////////////////////////////////
//
void SplashLayer::demo() {
  auto ps= mapGridPos(scale);
  auto scale= 0.75;
  auto fm= "";

  // we scale down the icons to make it look nicer
  for (auto i = 0; i < ps.size(); ++i) {
    // set up the grid icons
    if (i == 1 || i == 5 || i == 6 || i == 7) { fm= "x.png"; }
    else if (i == 0 || i == 4) { fm= "z.png"; }
    else { fm= "o.png"; }
    auto sp= cx::reifySprite(fm);
    auto bx= cx::vboxMID( ps[i]);
    sp->setScale(scale);
    sp->setPosition(bx.x, bx.y);
    addAtlasItem("game-pics", sp);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void SplashLayer::onPlay(c::Ref* rr) {
  auto f= []() { cx::runScene(XCFG()->startWith()); };
  auto m = MainMenu::reifyWithBackAction(f);
  cx::runScene( m);
}

//////////////////////////////////////////////////////////////////////////////
//
f::XLayer* SplashLayer::realize() {

  centerImage("game.bg");
  incIndexZ();
  regoAtlas("game-pics");

  // title
  auto cw = cx::center();
  auto wb = cx::visBox();
  addAtlasFrame("game-pics", "title.png",
                     c::Vec2(cw.x, wb.top * 0.9));

  demo();

  // play button
  auto menu= f::ReifyRefType<cocos2d::Menu>();
  auto b1= cx::reifyMenuBtn("play.png");
  b1->setTarget(getParent(),
      CC_MENU_SELECTOR(SplashLayer::onPlay));
  menu->addChild(b1);
  menu->setPosition( cw.x, wb.top * 0.1);
  addItem(menu);

  return this;
}
END_NS_UNAMED()

//////////////////////////////////////////////////////////////////////////////
//
f::XScene* Splash::realize() {
  auto y = f::ReifyRefType<SplashLayer>();
  addLayer(y);
  y->realize();
  return this;
}



NS_END(tttoe)
