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

  auto MARGIN = 26 / CC_CONTENT_SCALE_FACTOR();
  auto coin1 = cx::reifySprite("coin.png");
  auto coin2 = cx::reifySprite("coin.png");
  auto wb= cx::visBox();

  _stateLabel = cx::reifyBmfLabel("dft", "Idle");
  CC_POS2(_stateLabel, wb.cx, wb.top * 0.25);
  addItem(_stateLabel);

  CC_POS2(coin1,
      MARGIN + HWZ(CC_CSIZE(coin1)),
      wb.top - MARGIN - HHZ(CC_CSIZE(coin1)));
  addItem(coin1);

  CC_POS2(coin2,
      wb.right - MARGIN - HWZ(CC_CSIZE(coin2)),
      wb.top - MARGIN - HHZ(CC_CSIZE(coin2)));
  addItem(coin2);

  auto coin1Label = cx::reifyBmfLabel("btns","10");
  _coinLabels[1]= coin1Label;

  coin1Label->setAlignment(c::TextHAlignment::LEFT);
  CC_POS2(coin1Label,
      coin1->getPositionX() + HWZ(CC_CSIZE(coin1)) + MARGIN/2 +
      HWZ(CC_CSIZE(coin1Label)),
      wb.top - MARGIN*1.6);
  addItem(coin1Label);

  auto coin2Label = cx::reifyBmfLabel("btns", "10");
  _coinLabels[2]= coin2Label;
  coin2Label->setAlignment(c::TextHAlignment::RIGHT);
  CC_POS2(coin2Label, coin2->getPositionX() - HWZ(CC_CSIZE(coin1)) - MARGIN/2 -
      HWZ(CC_CSIZE(coin2Label)),
      wb.top - MARGIN*1.6);
  addItem(coin2Label);

  _scoreLabel= cx::reifyBmfLabel("dft", "0");
  CC_POS2(_scoreLabel, wb.cx, wb.top * 0.9);
  addItem(_scoreLabel);

  _coins.fill(10);
  _score=0;
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateAIState(const sstr &msg) {
  _stateLabel->setString(msg);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateCoins(int p, int v) {
  _coins[p]=v;
  _coinLabels[p]->setString(FTOS(v));
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScore(int n) {
  _score += n;
  _scoreLabel->setString(FTOS(_score));
}


NS_END



