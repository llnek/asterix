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

NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL UILayer : public f::XLayer {
  STATIC_REIFY_LAYER(UILayer)
  virtual ~UILayer() {}
  UILayer() {}
  NOCPYASS(UILayer)
  virtual void decorate();
};

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::decorate() {
  auto cw = cx::center();
  auto wb = cx::visBox();

  centerImage("game.bg");

  addFrame("title.png",
      c::ccp(cw.x, wb.top * 0.9f));

  auto b = cx::reifyMenuBtn("play.png");
  auto menu = cx::mkMenu(b);
  b->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onPlay));
  menu->setPosition(cw.x, wb.top * 0.1f);
  addItem(menu);
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::onPlay() {
  auto f = [=]() {
    cx::runScene(XCFG()->prelude(),
        CC_CSV(c::Float,"SCENE_DELAY"));
  };
  cx::runScene(MMenu::reify(mc_new_1(MCX,f)),
      CC_CSV(c::Float,"SCENE_DELAY"));
}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
void Splash::decorate() {
  UILayer::reify(this);
}




NS_END(pong)

