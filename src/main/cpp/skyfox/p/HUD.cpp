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
#include "n/C.h"
#include "HUD.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(skyfox)

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateEnergy(float v) {
  auto e=(int)v;
  _energyLabel->setString( FTOS(e) + "% ");
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScore(int v) {
  _score += v;
  _scoreLabel->setString( FTOS(_score) );
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::decoUI() {
  auto wb = cx::visBox();

  regoAtlas("game-pics");

  _scoreLabel= cx::reifyBmfLabel("dft", "0");
  _scoreLabel->setAnchorPoint(cx::anchorR());
  _scoreLabel->setPosition(wb.right * 0.8, wb.top * 0.94);
  addItem(_scoreLabel);

  _energyLabel = cx::reifyBmfLabel("dft", "100%");
  _energyLabel->setAlignment(c::TextHAlignment::RIGHT);
  _energyLabel->setPosition(wb.right * 0.3, wb.top * 0.94);
  addItem(_energyLabel);

  auto icon = cx::reifySprite("health_icon.png");
  icon->setPosition(wb.right * 0.15, wb.top * 0.94);
  addAtlasItem("game-pics",icon, kBackground);

  _score=0;
}


NS_END



