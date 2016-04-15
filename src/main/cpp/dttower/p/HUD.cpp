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
NS_BEGIN(dttower)

//////////////////////////////////////////////////////////////////////////
//
void HUDLayer::decoUI() {

  auto wb= cx::visBox();

  _score=0;

  _scoreLabel= cx::reifyBmfLabel("dft", "0");
  CC_POS2(_scoreLabel, wb.cx, wb.top * 0.9);
  XCFG()->scaleBmfont(_scoreLabel, 24);
  addItem(_scoreLabel);

  _waveLabel = cx::reifyBmfLabel("dft", "Wave " + FTOS(0));
  XCFG()->scaleBmfont(_waveLabel, 24);
  CC_POS2(_waveLabel, 40 , wb.top - 25);
  _waveLabel->setColor(cx::black());
  addItem(_waveLabel);

  _lifePointsLabel = cx::reifyBmfLabel("dft", "Life: " + FTOS(10) + " ");
  CC_POS2(_lifePointsLabel, wb.right - 50 , 20);
  XCFG()->scaleBmfont(_lifePointsLabel, 24);
  _lifePointsLabel->setColor(cx::black());
  addItem(_lifePointsLabel);

}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateLifePts(int p) {
  _lifePointsLabel->setString( "Life: " + FTOS(p) +" ");
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateWave(int w) {
  _waveLabel->setString("Wave " + FTOS(w));
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScore(int n) {
  _score += n;
  _scoreLabel->setString(FTOS(_score));
}


NS_END



