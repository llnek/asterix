// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Ken Leung. All rights reserved.

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Splash.h"
#include "MMenu.h"
#include "n/lib.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
//
void Splash::decoUI() {
  auto wz = cx::visRect();
  auto wb = cx::visBox();

  centerImage("game.bg");

  _flare = c::Sprite::create("pics/flare.jpg");
  CC_HIDE(_flare);
  _ship = cx::reifySprite("ship03.png");
  _ship->setPosition(cx::randFloat(wz.size.width), 0);
  addChild(_flare, 15, 10);
  addChild(_ship, 0, 4);

  auto b= cx::reifyMenuBtn("play.png");
  auto f= []() { cx::prelude(); };
  auto menu= cx::mkMenu(b);
  auto x= mc_new1(MCX,f);
  b->setCallback([=](c::Ref*) {
    btnEffect();
    flareEffect(_flare, [=]() {
      cx::runEx(MMenu::reify(x));
    });
  });

  b->setPosition(wb.cx, wb.top * 0.1);
  addItem(menu);

  scheduleOnce(CC_SCHEDULE_SELECTOR(Splash::update),0);
  cx::sfxMusic("mainMusic", true);
}

//////////////////////////////////////////////////////////////////////////////
//
void Splash::update(float dt) {
  auto wz = cx::visRect();
  auto g= [=]() {
    this->_ship->setPosition( cx::randFloat(wz.size.width), 10);
    this->update(0);
  };
  this->_ship->runAction(
      c::Sequence::create(
        c::MoveBy::create(2, c::Vec2(cx::randFloat(wz.size.width),
            wz.size.height + 100)),
        c::CallFunc::create(g),
        CC_NIL));
}


NS_END


