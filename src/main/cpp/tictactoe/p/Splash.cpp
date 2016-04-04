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
#include "n/C.h"
#include "MMenu.h"
#include "Splash.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(tttoe)

static s_arr<sstr,3> PNGS= {"z.png", "x.png", "o.png"};
static int PSZ= PNGS.size();
static float SCALEZ= 0.9;

//////////////////////////////////////////////////////////////////////////////
//
static const sstr GNP() {
  return PNGS[ rand() % PSZ ];
}

//////////////////////////////////////////////////////////////////////////////
//
static c::Sprite* mkIcon(const sstr &png) {
  auto sp= cx::reifySprite(png);
  sp->setScale(SCALEZ);
  return sp;
}

//////////////////////////////////////////////////////////////////////////////
//
void Splash::demo() {
  // we scale down the icons to make it look nicer
  auto ps= mapGridPos(SCALEZ);

  for (auto i = 0; i < ps.size(); ++i) {
    auto bx= cx::vboxMID( ps[i]);
    auto sp= mkIcon("z.png");
    sp->setPosition(bx);
    addAtlasItem("game-pics", sp);

    sp= mkIcon(GNP());
    sp->setPosition(bx);
    sp->runAction(
        c::RepeatForever::create(
        c::Sequence::create(
          c::FadeIn::create(cx::rand() * 5),
          c::DelayTime::create(cx::rand()*5),
          c::CallFuncN::create([=](c::Node *r) {
            SCAST(c::Sprite*,r)->setSpriteFrame(GNP());
            }),
          c::DelayTime::create(cx::rand() *5),
          c::FadeOut::create(cx::rand() * 5),
          c::DelayTime::create(cx::rand() * 5),
          c::CallFuncN::create([=](c::Node *r) {
            SCAST(c::Sprite*,r)->setSpriteFrame(GNP());
            }),
          CC_NIL)));

    addAtlasItem("game-pics", sp);
  }

  this->runAction(
      c::Sequence::create(
        c::DelayTime::create(0.1),
        c::CallFunc::create([=]() {
          this->decoUIXXX();
          }),
        CC_NIL));
}


//////////////////////////////////////////////////////////////////////////////
//
void Splash::decoUIXXX() {

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

  auto b1= cx::reifyMenuBtn("play.png");
  auto f= []() { cx::prelude(); };
  auto menu= cx::mkMenu(b1);
  b1->setCallback(
      [=](c::Ref*) {
        cx::runEx(MMenu::reify(mc_new1(MCX, f)));
        cx::sfxPlay("button");
      });

  b1->setPosition(wb.cx, wb.top * -0.15);
  b1->runAction(
      c::Sequence::createWithTwoActions(
        c::DelayTime::create(0.3),
        c::EaseBackOut::create(
          c::MoveTo::create(0.5,
            c::Vec2(wb.cx, wb.top * 0.1)))));

  addItem(menu);
}

//////////////////////////////////////////////////////////////////////////////
//
void Splash::decoUI() {

  centerImage( "game.bg");
  regoAtlas("game-pics");

  demo();
}



NS_END

