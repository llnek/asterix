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
NS_BEGIN(stoneage)

//////////////////////////////////////////////////////////////////////////////
//
void Splash::decorate() {

  auto wb=cx::visBox();

  centerImage("intro.bg");

  //create pterodactyl animation
  auto p= cx::loadSprite("ptero_frame1.png");
  p->setPosition(wb.right + 100, wb.top * 0.8f);
  addItem(p);

  auto animation = c::Animation::create();
  for (auto i=1; i < 4; ++i) {
    auto name = "pics/ptero_frame"+s::to_string(i)+".png";
    animation->addSpriteFrameWithFile(name);
  }
  animation->setDelayPerUnit(0.5 / 3.0);
  animation->setRestoreOriginalFrame(true);
  animation->setLoops(-1);

  auto animate = c::Animate::create(animation);
  p->runAction(animate);

  auto moveOut = c::MoveTo::create(0,
      c::Vec2(wb.right + 100, wb.top * 0.8));
  auto moveIn = c::MoveTo::create(4.0,
      c::Vec2(-100, wb.top * 0.8));
  auto delay = c::DelayTime::create(2.5);
  p->runAction(c::RepeatForever::create(
        c::Sequence::create(moveOut, moveIn, delay,CC_NIL)));

  auto ch= cx::loadSprite("introCharacter.png");
  ch->setPosition(wb.cx, wb.cy + 110);
  addItem(ch);

  auto frame = cx::loadSprite("frame.png");
  frame->setPosition(wb.cx, wb.cy);
  addItem(frame);

  auto btn = c::MenuItemImage::create("pics/playBtn.png", "pics/playBtnOver.png");
  btn->setPosition(wb.cx, wb.top * 0.2);
  btn->setCallback([=](c::Ref*) {
    cx::sfxMusic("background",true);
    cx::runEx(Game::reify(mc_new(f::GCX)));
  });
  auto menu  = cx::mkMenu(btn);
  addItem(menu);

}



NS_END


