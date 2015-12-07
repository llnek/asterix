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
#include "Splash.h"
#include "Menu.h"
#include "s/utils.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
//
BEGIN_NS_UNAMED()
class CC_DLL UILayer : public f::XLayer {
protected:

  void onPlay(c::Ref*);

  NOCPYASS(UILayer)
  IMPLCZ(UILayer)

  c::Sprite* flare;
  c::Sprite* ship;

public:

  virtual f::XLayer* realize();
  virtual void update(float);

  STATIC_REIFY_LAYER(UILayer)
};

//////////////////////////////////////////////////////////////////////////////
//
f::XLayer* UILayer::realize() {
  auto wz = cx::visRect();
  auto wb = cx::visBox();
  auto cw = cx::center();

  centerImage("game.bg");

  auto b= cx::reifyMenuBtn("play.png");
  auto menu= cx::mkMenu(b);
  b->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onPlay));
  menu->setPosition( cw.x, wb.top * 0.1f);
  addItem(menu);

  flare = cx::reifySprite("flare.png");
  flare->setVisible(false);
  ship = cx::reifySprite("ship03.png");
  ship->setPosition( cx::randInt(wz.width), 0);
  addItem(flare, 15, 10);
  addItem(ship, 0, 4);
  this->update();
  cx::sfxMusic("mainMusic", true);
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::onPlay(c::Ref*) {
  auto f= [=]() { cx::runScene(XCFG()->startWith()); };
  btnEffect();
  flareEffect(flare, [=]() {
    cx::runScene(MainMenu::reifyWithBackAction(f));
  });
}

//////////////////////////////////////////////////////////////////////////////
//
void UILayer::update(float dt) {
  auto wz = cx::visRect();
  auto g= [=]() {
    this->ship->setPosition( cx::randInt(wz.width), 10);
    this->update();
  };
  this->ship->runAction(c::Sequence::create(
        c::MoveBy::create(2,
          ccp(cx::randInt(wz.width), wz.height + 100)),
        c::CallFunc::create(g)));
}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
f::Scene* Splash::realize() {
  addLayer(UILayer::reify())->realize();
  return this;
}

NS_END(terra)


