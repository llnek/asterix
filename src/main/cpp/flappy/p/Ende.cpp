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
#include "Ende.h"
#include "HUD.h"
#include "n/lib.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(flappy)

//////////////////////////////////////////////////////////////////////////////
//
void Ende::decoUI() {

  auto wz=cx::visRect();
  auto wb=cx::visBox();
  auto popup = c::LayerColor::create(
      c::Color4B(0, 0, 0, 196),
      CC_ZW(wz.size), CC_ZH(wz.size));

  popup->setOpacity(0);
  popup->runAction(c::FadeTo::create(0.25, 196));
  addItem(popup);

  auto gameOverSprite = cx::reifySprite("dhgover");
  gameOverSprite->setPosition(wb.cx, wb.top * 0.75);
  addItem(gameOverSprite);

  auto restart= cx::reifyMenuBtn("dhplay");
  restart->setCallback([=](c::Ref*) {
      cx::runEx(Game::reify( new f::GCX() ));
      });

  auto menu = cx::mkMenu(restart);
  menu->setPosition(wb.cx, wb.cy);
  addItem(menu);

  auto score= getHUD()->getScore();
  auto scoreLabel = cx::reifyLabel("dft", 60, "Score: " + FTOS(score));
  scoreLabel->setPosition(wb.cx, wb.top * 0.6);
  scoreLabel->runAction(
      c::Sequence::create(
        c::DelayTime::create(0.5),
        c::EaseSineIn::create(c::ScaleTo::create(0.25, 1.1)),
        c::EaseSineOut::create(c::ScaleTo::create(0.25, 1)),
        CC_NIL));
  addItem(scoreLabel);

  auto oldHighScore = CC_APPDB()->getIntegerForKey(HIGHSCORE_KEY);
  auto highScoreLabel = cx::reifyLabel("dft", 60, "Your Best: " + FTOS(oldHighScore));
  highScoreLabel->setPosition(wb.cx, wb.cy);
  addItem(highScoreLabel);

  // check if new high score has been achieved
  if (score > oldHighScore) {
    // save the new high score
    CC_APPDB()->setIntegerForKey(HIGHSCORE_KEY, score);
    CC_APPDB()->flush();
    highScoreLabel->runAction(
        c::Sequence::create(
          c::DelayTime::create(1),
          c::EaseSineIn::create(c::ScaleTo::create(0.25, 1.1)),
          c::CallFunc::create([=]() {
             highScoreLabel->setString("Your Best: " + FTOS(score));
            }),
          c::EaseSineOut::create(c::ScaleTo::create(0.25, 1)),
          CC_NIL));
  }
}


NS_END



