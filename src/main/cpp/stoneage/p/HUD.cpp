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
#include "HUD.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(stoneage)

//////////////////////////////////////////////////////////////////////////
//
void HUDLayer::decorate() {

  auto diamondScoreBg = cx::loadSprite("diamondScore.png");
  auto scoreBg = cx::loadSprite("gemsScore.png");
  auto wb= cx::visBox();

  diamondScoreBg->setPosition(100, wb.top - 30);
  addItem(diamondScoreBg);

  scoreBg->setPosition(280, wb.top - 30);
  addItem(scoreBg);

  dscoreLabel = cx::reifyLabel("dft", 20, "0");
    dscoreLabel->setAlignment(c::TextHAlignment::RIGHT);
  dscoreLabel->setMaxLineWidth(150);
  dscoreLabel->setPosition (140, wb.top - 30);
  addItem(dscoreLabel);

  scoreLabel = cx::reifyLabel("dft", 20, "0");
    scoreLabel->setAlignment(c::TextHAlignment::RIGHT);
  scoreLabel->setMaxLineWidth(150);
  scoreLabel->setPosition (330, wb.top - 30);
  addItem(scoreLabel);

  dscore=0;
  score=0;
}


NS_END



