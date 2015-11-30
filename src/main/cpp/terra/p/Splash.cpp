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

  NO__CPYASS(UILayer)
  UILayer()=delete;

  c::Sprite* flare;
  c::Sprite* ship;

public:

  virtual f::XLayer* realize();
  virtual void update(float);

  IMPL_CTOR(UILayer)
};

//////////////////////////////////////////////////////////////////////////////
//
f::XLayer* UILayer::realize() {
  auto b= cx::reifyMenuBtn("play.png");
  auto wz = cx::visRect();
  auto wb = cx::visBox();
  auto cw = cx::center();

  centerImage("game.bg");

  b->setTarget(this,
      CC_MENU_SELECTOR(UILayer::onPlay));

  auto menu= cx::mkMenu(b);
  menu->setPosition( cw.x, wb.top * 0.1f);
  addItem(menu);

  flare = cx::reifySprite("flare.png");
  flare->setVisible(false);
  ship = cx::reifySprite("ship03.png");
  ship->setPosition(
    CC_RANDOM_1(wz.width),
    0
  );
  addItem(flare, 15, 10);
  addItem(ship, 0, 4);
  this->update();
  cx::sfxMusic("mainMusic", true);
  //this.schedule(this.update, 0.1);
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
  auto g= cx::CallFunc::create([=]() {
    this->ship->setPosition(
      sjs.rand(wz.width),
      10
    );
    this->update();
  });
  this->ship->runAction(c::Sequence::create(
        c::MoveBy::create(2, ccp(CC_RANDOM_1(wz.width),
                                             wz.height + 100)), g));
}

END_NS_UNAMED()

//////////////////////////////////////////////////////////////////////////////
//
f::Scene* Splash::realize() {
  auto y = f::reifyRefType<UILayer>();
  addLayer(y);
  y->realize();
  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
Splash* Splash::reify() {
  auto s = f::reifyRefType<Splash>();
  s->realize();
  return s;
}


NS_END(terra)


