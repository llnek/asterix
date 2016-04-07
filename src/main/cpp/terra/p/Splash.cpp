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
#include "Game.h"
#include "n/C.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
//
void Splash::decoUI() {

  auto wb = cx::visBox();

  centerImage("gui.xxx.bg");
  regoAtlas("game-pics");

  _ship = cx::reifySprite("ship03.png");
  _ship->setScale(XCFG()->getScale());
  _flare = cx::createSprite("flare");

  CC_HIDE(_flare);
  CC_POS2(_ship, cx::randInt(wb.right), 0);

  addItem(_flare, 15, 10);
  addItem(_ship, 0, 4);

  auto b= cx::reifyMenuText("btns", "PLAY!");
  CC_POS2(b, wb.cx, wb.top * 0.2);
  b->setCallback([=](c::Ref*) {
    btnEffect();
    flareEffect(_flare, [=]() {
      cx::runEx(Game::reify(mc_new(GameCtx)));
    });
  });

  addItem(cx::mkMenu(b));

  scheduleOnce(CC_SCHEDULE_SELECTOR(Splash::update),0);
  cx::sfxMusic("mainMusic", true);
}

//////////////////////////////////////////////////////////////////////////////
//
void Splash::update(float dt) {
  auto h= CC_CHT(_ship) * 3;
  auto wb = cx::visBox();
  auto g= [=]() {
    CC_POS2(this->_ship, cx::randInt(wb.right), 10);
    this->update(0);
  };
  this->_ship->runAction(
      c::Sequence::create(
        c::MoveBy::create(
          2,
          CCT_PT(cx::randInt(wb.right), wb.top + h)),
        c::CallFunc::create(g),
        CC_NIL));
}


NS_END


