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
#include "BackDrop.h"
#include "Splash.h"
#include "Game.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(spacecraze)

//////////////////////////////////////////////////////////////////////////////
//
void Splash::decoUI() {

  auto title = cx::reifySprite("sftitle");
  auto wz= cx::visRect();
  auto wb= cx::visBox();

  BackDrop::reify(this, -1);

  regoAtlas("game-pics");

  title->setPosition(wb.cx, wb.top * 1.2);
  title->runAction(
      c::Sequence::create(
        c::DelayTime::create(0.5),
        c::EaseBackOut::create(
          c::MoveBy::create(0.5,
            c::Vec2(wb.left, wb.top * -0.5))),
        CC_NIL));
  addAtlasItem("game-pics", title);

  auto b= cx::reifyMenuBtn("play");
  auto menu= cx::mkMenu(b);
  auto ctx= mc_new(f::GCX);

  b->setPosition(wb.cx, wb.top * -0.15);
  b->runAction(
      c::Sequence::create(
        c::DelayTime::create(1),
        c::EaseBackOut::create(
          c::MoveBy::create(0.5,
            c::Vec2(wb.left, HTV(wb.top)))),
        CC_NIL));
  b->setCallback(
      [=](c::Ref*) { cx::runEx(Game::reify(ctx)); });
  addItem(menu);

  demo(0);
  schedule(CC_SCHEDULE_SELECTOR(Splash::demo), 0.5);
}

//////////////////////////////////////////////////////////////////////////////
//
void Splash::demo(float dt) {

  auto duration = cx::randInt(4);
  auto wb = cx::visBox();
  c::Vec2 dist;
  c::Vec2 src;

  if (cx::randSign() > 0) {
    // left-to-right
    src.x = cx::randSign() * wb.right * 0.25;
    src.y = wb.top * 0.25 + cx::rand() * HTV(wb.top);
    dist.x = src.x < 0 ? wb.right*1.5 : wb.right*-1.5;
    dist.y = cx::rand() * wb.top;
  } else {
    // top-to-bottom
    src.x = wb.right * 0.25 + cx::rand() * HTV(wb.right);
    src.y = cx::randSign() * wb.top * 0.25;
    dist.x = cx::rand() * wb.right;
    dist.y = src.y < 0 ? wb.top*1.5 : wb.top*-1.5;
  }

  // choose an enemy ship 1 <= x <= 3
  int type = 1 + (int)floor(cx::rand() * 3);
  auto png = "sfenmy" + s::to_string(type);
  auto enemy = cx::reifySprite(png);

  addAtlasItem("game-pics", enemy);
  enemy->setPosition(src);

  // create & run sequence of move & remove
  enemy->runAction(
      c::Sequence::create(
        c::MoveBy::create(duration, dist),
        c::RemoveSelf::create(true),
        CC_NIL));
}


NS_END

