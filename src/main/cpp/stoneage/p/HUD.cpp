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
#include "HUD.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(stoneage)

//////////////////////////////////////////////////////////////////////////
//
void HUDLayer::decoUI() {

  auto diamondScoreBg = cx::reifySprite("diamondScore.png");
  auto scoreBg = cx::reifySprite("gemsScore.png");
  auto wb= cx::visBox();

  CC_POS2(diamondScoreBg, 100, wb.top - 30);
  addItem(diamondScoreBg);

  CC_POS2(scoreBg, 280, wb.top - 30);
  addItem(scoreBg);

  _dscoreLabel = cx::reifyLabel("text", 20, "0");
  _dscoreLabel->setAlignment(c::TextHAlignment::RIGHT);
  _dscoreLabel->setMaxLineWidth(150);
  CC_POS2(_dscoreLabel, 140, wb.top - 30);
  addItem(_dscoreLabel);

  _scoreLabel = cx::reifyLabel("text", 20, "0");
  _scoreLabel->setAlignment(c::TextHAlignment::RIGHT);
  _scoreLabel->setMaxLineWidth(150);
  CC_POS2(_scoreLabel, 330, wb.top - 30);
  addItem(_scoreLabel);

  _dscore=0;
  _score=0;
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScore(const sstr &type, int value) {
  if ("diamond" == type) {
    _dscore += value;
    _dscoreLabel->setString(FTOS(_dscore));
  } else {
    _score += value;
    _scoreLabel->setString(FTOS(_score));
  }
}



NS_END



