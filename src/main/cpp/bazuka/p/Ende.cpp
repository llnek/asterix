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

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(bazuka)

//////////////////////////////////////////////////////////////////////////////
//
void Ende::decoUI() {

  auto btn = cx::reifyMenuBtn("mainmenu.png");
  auto menu= cx::mkMenu(btn);
  auto wb= cx::visBox();

  btn->setPosition(wb.cx, wb.top * 0.2);
  btn->setCallback([=](c::Ref*){
    cx::sfxPlay("pop");
    cx::runEx(Splash::reify());
  });
  addItem(menu);

  auto gameOverLabel = cx::reifyBmfLabel("dft", "GAMEOVER");
  gameOverLabel->setPosition(wb.cx, wb.top * 0.6);
  addItem(gameOverLabel, 10);

  auto highScore = CC_APPDB()->getIntegerForKey("GameHighScore");
  auto score= getHUD()->getScore();

  if (score > highScore) {
    CC_APPDB()->setIntegerForKey("GameHighScore", score);
    CC_APPDB()->flush();

    auto newHighScoreLabel = cx::reifyBmfLabel("dft", "NEW HIGH SCORE");
    newHighScoreLabel->setPosition(wb.cx, wb.cy);
    newHighScoreLabel->setScale(0.75);
    addItem(newHighScoreLabel, 10);

    auto GOscoreLabel = cx::reifyBmfLabel("dft", s::to_string(score));
    GOscoreLabel->setPosition(wb.cx, wb.top * 0.4);
    GOscoreLabel->setScale(0.75);
    addItem(GOscoreLabel, 10);
  } else {
    auto newHighScoreLabel = cx::reifyBmfLabel("dft", "BETTER LUCK NEXT TIME");
    newHighScoreLabel->setPosition(wb.cx, wb.cy);
    newHighScoreLabel->setScale(0.75);
    addItem(newHighScoreLabel, 10);
  }

}


NS_END



