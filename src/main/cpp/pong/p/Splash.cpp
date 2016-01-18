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
#include "x2d/XLayer.h"
#include "Splash.h"
#include "MMenu.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL UILayer : public f::XLayer {
  STATIC_REIFY_LAYER(UILayer)
  MDECL_DECORATE()
protected:
  void onPlay(c::Ref*);
};

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::decorate() {
  auto cw = cx::center();
  auto wb = cx::visBox();

  centerImage("game.bg");
  incIndexZ();

  addFrame("title.png", c::ccp(cw.x, wb.top * 0.9f));

  auto b = cx::reifyMenuBtn("play.png");
  auto menu = cx::mkMenu(b);
  b->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onPlay));
  menu->setPosition(cw.x, wb.top * 0.1f);
  addItem(menu,lastZ, ++lastTag);
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::onPlay(c::Ref*) {
  auto f = [=]() { cx::runEx( XCFG()->prelude()); };
  cx::runEx( MMenu::reify(mc_new1(MCX,f)));
}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
void Splash::decorate() {
  UILayer::reify(this);
}




NS_END(pong)

