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

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(spacecraze)

//////////////////////////////////////////////////////////////////////////////
//
void Splash::decorate() {

  auto title = cx::reifySprite("sftitle");
  auto wz= cx::visRect();
  auto wb= cx::visBox();

  regoAtlas("game-pics");

  title->setPosition(wb.cx, wz.height * 1.2);
  title->runAction(
      c::Sequence::createWithTwoActions(
        c::DelayTime::create(0.5f),
        c::EaseBackOut::create(
          c::MoveBy::create(0.5f,
            c::ccp(0, wz.height * -0.5f)))));
  addAtlasItem("game-pics", title);

  auto b= cx::reifyMenuBtn("play");
  auto menu= cx::mkMenu(b);
  auto ctx= mc_new(f::GCX);

  b->setPosition(wb.cx, wb.top * -0.15);
  b->runAction(
      c::Sequence::createWithTwoActions(
        c::DelayTime::create(1.0f),
        c::EaseBackOut::create(
          c::MoveBy::create(0.5f,
            c::ccp(0, wz.top * 0.5f)))));

  b->setCallback(
      [=](c::Ref*) { cx::runEx(Game::reify(ctx)); });
  addItem(menu);

  demo(0);
  schedule(CC_SCHEDULE_SELECTOR(Splash::demo), 0.5f);
}

//////////////////////////////////////////////////////////////////////////////
//
void Splash::demo(float dt) {

  auto duration = cx::randInt(4);
  auto wz = cx::visRect();
  c::Vec2 dist;
  c::Vec2 src;

  if(cx::randSign() > 0) {
    // left-to-right
    src.x = cx::randSign() * wz.width * 0.25f;
    src.y = wz.height * 0.25f + cx::rand() * wz.height * 0.5f;
    dist.x = src.x < 0 ? wz.width*1.5f : wz.width*-1.5f;
    dist.y = cx::rand() * wz.height;
  } else {
    // top-to-bottom
    src.x = wz.width * 0.25f + cx::rand() * wz.width * 0.5f;
    src.y = cx::randSign() * wz.height * 0.25f;
    dist.x = cx::rand() * wz.width;
    dist.y = src.y < 0 ? wz.height*1.5f : wz.height*-1.5f;
  }

  // choose an enemy ship 1 <= x <= 3
  int type = 1 + (int)floor(cx::rand() * 3);
  auto png = "sfenmy" + s::to_string(type);
  auto enemy = cx::reifySprite(png);

  addAtlasItem("game-pics", enemy);
  enemy->setPosition(src);

  // create & run sequence of move & remove
  auto move= c::MoveBy::create(duration, dist);
  auto remove = c::RemoveSelf::create(true);
  enemy->runAction(c::Sequence::createWithTwoActions(move, remove));

}


NS_END

