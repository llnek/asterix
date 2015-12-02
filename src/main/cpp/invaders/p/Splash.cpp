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
NS_BEGIN(invaders)

BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL UILayer : public f::XLayer {
protected:
  virtual f::XLayer* realize();
  void onPlay(c::Ref*);
  NOCPYASS(UILayer)
  IMPL_CTOR(UILayer)
public:
  STATIC_REIFY_LAYER(UILayer)
};

//////////////////////////////////////////////////////////////////////////
//
f::XLayer* UILayer::realize() {

  auto wb = cx::visBox();
  auto cw = cx::center();

  centerImage("game.bg");

  addFrame("title.png", c::Vec2(cw.x, wb.top * 0.9f));

  auto b1 = cx::reifyMenuBtn("play.png");
  auto menu = cx::mkMenu(b1);
  b1->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onPlay));
  menu->setPosition( cw.x, wb.top * 0.1f);
  addItem(menu);

  return this;
}

//////////////////////////////////////////////////////////////////////////
//
void UILayer::onPlay(c::Ref* b) {
  auto a= [=]() {
      cx::runScene(XCFG()->startWith());
      };
  cx::runScene( MainMenu::reifyWithBackAction(a));
}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
f::XScene* Splash::realize() {
  addLayer(UILayer::reify())->realize();
  return this;
}


NS_END(invaders)

