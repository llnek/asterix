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
#include "n/GNodes.h"
#include "s/utils.h"
#include "Menu.h"
#include "Splash.h"
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL UILayer : public f::XLayer {

  STATIC_REIFY_LAYER(UILayer)

  virtual void decorate();

  virtual ~UILayer() {}
  UILayer() {}

  void onPlay(c::Ref*);
  void demo();

  NOCPYASS(UILayer)

};

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::demo() {
  auto scale= 0.75;
  auto fm= "";
  auto ps= mapGridPos(scale);

  // we scale down the icons to make it look nicer
  for (int i = 0; i < ps.size(); ++i) {
    // set up the grid icons
    if (i == 1 || i == 5 || i == 6 || i == 7) { fm= "x.png"; }
    else if (i == 0 || i == 4) { fm= "z.png"; }
    else { fm= "o.png"; }
    auto sp= cx::reifySprite(fm);
    auto bx= cx::vboxMID( ps[i]);
    sp->setScale(scale);
    sp->setPosition(bx);
    addAtlasItem("game-pics", sp);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::onPlay(c::Ref*) {
  auto f= [=]() { cx::runScene(XCFG()->prelude()); };
  auto m = MMenu::reify(mc_new_1(MCX, f));
  cx::runScene( m, CC_CSV(c::Float, "SCENE_DELAY"));
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::decorate() {

  centerImage("game.bg");
  incIndexZ();
  regoAtlas("game-pics");

  // title
  auto cw = cx::center();
  auto wb = cx::visBox();
  addAtlasFrame("game-pics", "title.png",
                     c::Vec2(cw.x, wb.top * 0.9f));

  demo();

  // play button
  auto b1= cx::reifyMenuBtn("play.png");
  auto menu= cx::mkMenu(b1);
  b1->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onPlay));
  menu->setPosition( cw.x, wb.top * 0.1f);
  addItem(menu);

}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
void Splash::decorate() {
  UILayer::reify(this);
}


NS_END(tttoe)

