// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Splash.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(blast)

//////////////////////////////////////////////////////////////////////////////
//
void Splash::decoUI() {

  auto background= f::reifyRefType<BackgroundManager>();
  addItem(background, E_LAYER_BACKGROUND);

  auto title = cx::reifySprite("iutitle");
  auto wb= cx::visBox();

  title->setPosition(wb.cx, wb.top* 1.2);
  title->runAction(
      c::Sequence::create(
        c::DelayTime::create(0.5),
        c::EaseBackOut::create(
          c::MoveBy::create(0.5, c::Vec2(0, -wb.cy))),
        CC_NIL));
  addChild(title, E_LAYER_FOREGROUND);

  auto play = cx::reifyMenuBtn("play_button.png");
  auto menu= cx::mkMenu(play);
  play->setColor(c::ccc3(77, 210, 245));
  play->setPosition(wb.cx, wb.top * -0.15);
  play->runAction(
      c::Sequence::create(
        c::DelayTime::create(1),
        c::EaseBackOut::create(
          c::MoveBy::create(0.5, c::Vec2(0, wb.cy))),
        CC_NIL));
  play->setCallback([=](c::Ref*){
      cx::runEx(Game::reify( new GameCtx()  ));
      });
  addItem(menu, E_LAYER_FOREGROUND);

}


NS_END


