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

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Splash.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(cuteness)

//////////////////////////////////////////////////////////////////////////////
//
void Splash::decoUI() {
  auto bg= centerImage("game.bg");
  bg->setOpacity(0);
  bg->runAction(
      c::Sequence::create(
        c::FadeIn::create(1),
        c::CallFunc::create([=]() { this->deco2(); }),
        CC_NIL));
  //regoAtlas("gpics");
}

//////////////////////////////////////////////////////////////////////////////
//
void Splash::deco2() {
  auto t = cx::reifySprite("gametitle.png");
  auto wb= cx::visBox();

  CC_POS2(t, wb.cx, wb.top * 0.8);
  t->setOpacity(0);
  t->setScale(4);
  t->runAction(c::Spawn::create(
        c::FadeIn::create(0.5) ,
        c::ScaleTo::create(0.5,1),
        CC_NIL
        ));
  addItem(t);

  auto p= cx::reifySprite("planet.png");
  CC_POS2(p, wb.cx, wb.cy);
  p->setScale(2);
  p->runAction(
      c::Sequence::create(
        c::ScaleTo::create(1,1),
        c::ScaleTo::create(0.5,1.5),
        c::ScaleTo::create(0.5,1),
        CC_NIL));
  addItem(p);

  auto b= cx::reifyMenuText("btns", "PLAY");
  XCFG()->scaleBmfont(b, 36);
  CC_POS2(b, wb.cx, wb.top * 0.2);
  b->setCallback([=](c::Ref*) {
      cx::runEx(Game::reify(mc_new(GameCtx)));
      });
  addItem(cx::mkMenu(b));
}


NS_END


