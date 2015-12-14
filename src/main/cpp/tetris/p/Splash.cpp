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
NS_BEGIN(tetris)


BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL UILayer : public f::XLayer {
protected:

public:

  STATIC_REIFY_LAYER(UILayer)
  virtual void decorate();
};

//////////////////////////////////////////////////////////////////////////
//
void UILayer::decorate() {

  auto b= cx::reifyMenuBtn("play.png");
  auto menu= cx::mkMenu(b);
  auto wb = cx::visBox();
  auto cw = cx::center();

  centerImage("game.bg");

  b->setPosition(cw.x, wb.top * 0.1);
  b->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onPlay));

  addItem(menu);

}

//////////////////////////////////////////////////////////////////////////
//
void UILayer::onPlay(c::Ref*) {
  auto f= [=]() {
    cx::runScene(
        XCFG()->startWith());
  };
  auto m= MainMenu::reify(mc_new_1(MContext, f));
  cx::runScene(m);
}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////
//
void Splash::decorate() {
  UILayer::reify(this);
}


NS_END(tetris)


