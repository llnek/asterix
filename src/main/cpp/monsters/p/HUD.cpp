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
NS_BEGIN(monsters)

//////////////////////////////////////////////////////////////////////////
//
void HUDLayer::decoUI() {

  auto _stateLabel = cx::reifyBmfLabel("dft", "Idle");
  auto MARGIN = 26 / CC_CONTENT_SCALE_FACTOR();
  auto coin1 = cx::reifySprite("coin.png");
  auto coin2 = cx::reifySprite("coin.png");
  auto wb= cx::visBox();

  _stateLabel->setPosition(wb.cx, wb.top * 0.25);
  addItem(_stateLabel);

  coin1->setPosition(
      MARGIN + HWZ(CC_CSIZE(coin1)),
      wb.top - MARGIN - HHZ(CC_CSIZE(coin1)));
  addItem(coin1);

  coin2->setPosition(
      wb.right - MARGIN - HWZ(CC_CSIZE(coin2)),
      wb.top - MARGIN - HHZ(CC_CSIZE(coin2)));
  addItem(coin2);

  auto _coin1Label = cx::reifyBmfLabel("dft","10");
  coinLabels[1]= _coin1Label;

  _coin1Label->setAlignment(c::TextHAlignment::LEFT);
  _coin1Label->setPosition(
      coin1->getPositionX() + HWZ(CC_CSIZE(coin1)) + MARGIN/2 +
      HWZ(CC_CSIZE(_coin1Label)),
      wb.top - MARGIN*1.6);
  addItem(_coin1Label);

  auto _coin2Label = cx::reifyBmfLabel("dft", "10");
  coinLabels[2]= _coin2Label;
  _coin2Label->setAlignment(c::TextHAlignment::RIGHT);
  _coin2Label->setPosition(coin2->getPositionX() - HWZ(CC_CSIZE(coin1)) - MARGIN/2 -
      HWZ(CC_CSIZE(_coin2Label)),
      wb.top - MARGIN*1.6);
  addItem(_coin2Label);


  scoreLabel= cx::reifyBmfLabel("dft", "0");
  scoreLabel->setPosition(wb.cx, wb.top * 0.9);
  addItem(scoreLabel);

  coins.fill(10);
  score=0;
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateAIState(const sstr &msg) {
  _stateLabel->setString(msg);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateCoins(int p, int v) {
  coins[p]=v;
  coinLabels[p]->setString(s::to_string(v));
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScore(int n) {
  score += n;
  scoreLabel->setString(s::to_string(score));
}


NS_END



