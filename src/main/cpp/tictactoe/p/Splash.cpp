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
#include "n/CObjs.h"
#include "s/utils.h"
#include "Menu.h"
#include "Splash.h"
NS_BEGIN(tttoe)

BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL UILayer : public f::XLayer {
public:
  virtual f::XLayer* realize();

  void onPlay(c::Ref*);
  void demo();

  NO__CPYASS(UILayer)
  IMPL_CTOR(UILayer)
};

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::demo() {
  auto scale= 0.75;
  auto fm= "";
  auto ps= mapGridPos(scale);

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
void UILayer::onPlay(c::Ref* rr) {
  auto f= []() { cx::runScene(XCFG()->startWith()); };
  auto m = MainMenu::reifyWithBackAction(f);
  cx::runScene( m);
}

//////////////////////////////////////////////////////////////////////////////
//
f::XLayer* UILayer::realize() {

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
  auto menu= f::reifyRefType<cocos2d::Menu>();
  auto b1= cx::reifyMenuBtn("play.png");
  b1->setTarget(getParent(),
      CC_MENU_SELECTOR(UILayer::onPlay));
  menu->addChild(b1);
  menu->setPosition( cw.x, wb.top * 0.1);
  addItem(menu);

  return this;
}
END_NS_UNAMED()

//////////////////////////////////////////////////////////////////////////////
//
f::XScene* Splash::realize() {
  auto y = f::reifyRefType<UILayer>();
  addLayer(y);
  y->realize();
  return this;
}


Splash* Splash::reify() {
  auto s = f::reifyRefType<Splash>();
  s->realize();
  return s;
}


NS_END(tttoe)
