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
NS_BEGIN(guesses)

//////////////////////////////////////////////////////////////////////////
//
void HUDLayer::decoUI() {

  auto wb= cx::visBox();
  float gap;

  _scoreLabel = cx::reifyBmfLabel("dft", "0");
  _moves=0;

  XCFG()->scaleNode(_scoreLabel,24);
  gap= CC_CHT(_scoreLabel)/GOLDEN_RATIO;
  _scoreLabel->setAlignment(c::TextHAlignment::CENTER);
  _scoreLabel->setAnchorPoint(cx::anchorBL());
  CC_POS2(_scoreLabel, wb.left + gap, wb.bottom + gap);
  updateMoves(0);
  addItem(_scoreLabel);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateMoves(int n) {
  _moves=n;
  _scoreLabel->setString("Moves: " + FTOS(_moves));
}


NS_END



