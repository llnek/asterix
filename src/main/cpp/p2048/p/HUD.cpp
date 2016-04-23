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
NS_BEGIN(p2048)

//////////////////////////////////////////////////////////////////////////
//
void HUDLayer::decoUI() {

  _scoreLabel = cx::reifyBmfLabel("dft", "SCORE: 0");
  _score=0;
  XCFG()->scaleBmfont(_scoreLabel,80);

  auto margin= 16 * XCFG()->getScale();
  auto wb= cx::visBox();
  CC_POS2(_scoreLabel, wb.left + margin,
                       wb.top - margin);
  addItem(_scoreLabel);

}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScore(int n) {
  _score += n;
  _scoreLabel->setString(FTOS(_score));
}


NS_END



