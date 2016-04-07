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
void Splash::decoUI() {

  auto wz= cx::visSize();
  auto wb=cx::visBox();

  centerImage("intro.bg");

  //create pterodactyl animation
  auto p= cx::reifySprite("ptero_frame1.png");
  CC_POS2(p, wb.right + 100, wb.top * 0.8);
  addItem(p);

  auto anim= cx::createAnimation(0.5/3.0,true,-1);
  for (auto i=1; i < 4; ++i) {
    anim->addSpriteFrame(
        cx::getSpriteFrame("ptero_frame"+FTOS(i)+".png"));
  }

  auto animate = c::Animate::create(anim);
  p->runAction(animate);

  p->runAction(c::RepeatForever::create(
        c::Sequence::create(
          c::MoveTo::create(0,
              CCT_PT(wb.right + 100, wb.top * 0.8)),
          c::MoveTo::create(4.0,
              CCT_PT(-100, wb.top * 0.8)),
          c::DelayTime::create(2.5),
          CC_NIL)));

  auto ch= cx::reifySprite("introCharacter.png");
  CC_POS2(ch, wb.cx, wb.cy + 110);
  addItem(ch);

  auto frame = cx::createSprite("frame");
  auto sz= CC_CSIZE(frame);
  frame->setScale(wz.width/sz.width);
  CC_POS2(frame, wb.cx, wb.cy);
  addItem(frame);

  auto btn = cx::reifyMenuBtn("playBtn.png",
                              "playBtnOver.png");
  CC_POS2(btn, wb.cx, wb.top * 0.2);
  btn->setCallback([=](c::Ref*) {
    cx::sfxMusic("background",true);
    cx::runEx(Game::reify(mc_new(GameCtx)));
  });

  addItem(cx::mkMenu(btn));

}



NS_END


