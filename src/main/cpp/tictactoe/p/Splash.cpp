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
#include "n/lib.h"
#include "MMenu.h"
#include "Splash.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
/*
void Splash::XXXdemo() {
  auto ps= mapGridPos(0.75);
  auto fm= "";

  // we scale down the icons to make it look nicer
  for (auto i = 0; i < ps.size(); ++i) {
    // set up the grid icons
    if (i == 1 || i == 5 || i == 6 || i == 7)
    { fm= "x.png"; }
    else
    if (i == 0 || i == 4)
    { fm= "z.png"; }
    else
    { fm= "o.png"; }
    auto sp= cx::reifySprite(fm);
    auto bx= cx::vboxMID( ps[i]);
    sp->setScale(0.75);
    sp->setPosition(bx);
    addAtlasItem("game-pics", sp);
  }
}
*/
static s_arr<sstr,3> PNGS= {"z.png", "x.png", "o.png"};

//////////////////////////////////////////////////////////////////////////////
//
void Splash::demo() {
  // we scale down the icons to make it look nicer
  auto ps= mapGridPos(0.75);
  auto fm= "";

  s::srand(cx::timeInMillis());

  for (auto i = 0; i < ps.size(); ++i) {
    auto n= rand() % 3;
    auto sp= cx::reifySprite(PNGS[n]);
    auto bx= cx::vboxMID( ps[i]);
    sp->setScale(0);
    sp->setPosition(bx);
    sp->runAction(
        c::RepeatForever::create(
        c::Sequence::create(
          c::ScaleTo::create(0.1,0.75),
          c::DelayTime::create(0.2),
          c::CallFuncN::create([=](c::Node *r) {
            SCAST(c::Sprite*,r)->setSpriteFrame(PNGS[rand() % 3]);
            }),
          c::DelayTime::create(1),
          c::ScaleTo::create(0.1,0),
          c::DelayTime::create(0.1),
          c::CallFuncN::create([=](c::Node *r) {
            SCAST(c::Sprite*,r)->setSpriteFrame(PNGS[rand() % 3]);
            }),
          CC_NIL)));

    icons.push_back(sp);
    addAtlasItem("game-pics", sp);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Splash::decoUI() {

  centerImage( "game.bg");
  regoAtlas("game-pics");

  auto title= cx::reifySprite("title.png");
  auto wb = cx::visBox();

  title->setPosition(wb.cx, wb.top * 1.2);
  title->runAction(
      c::Sequence::createWithTwoActions(
        c::DelayTime::create(0.3),
        c::EaseBackOut::create(
          c::MoveTo::create(0.5,
            c::ccp(wb.cx, wb.top * 0.9)))));

  addAtlasItem("game-pics", title);

  demo();

  auto b1= cx::reifyMenuBtn("play.png");
  auto f= []() { cx::prelude(); };
  auto menu= cx::mkMenu(b1);
  b1->setCallback(
      [=](c::Ref*) {
        cx::runEx(MMenu::reify(mc_new1(MCX, f)));
      });

  b1->setPosition(wb.cx, wb.top * -0.15);
  b1->runAction(
      c::Sequence::createWithTwoActions(
        c::DelayTime::create(0.3),
        c::EaseBackOut::create(
          c::MoveTo::create(0.5,
            c::Vec2(wb.cx, wb.top * 0.1)))));

  addItem(menu);

  //schedule(CC_SCHEDULE_SELECTOR(Splash::jiggle), 1);
}



NS_END

