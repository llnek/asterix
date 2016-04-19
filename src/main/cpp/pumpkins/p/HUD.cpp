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
NS_BEGIN(pumpkins)

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::decoUI() {
  auto sprite = cx::reifySprite("TD_hudbg.png");

  regoAtlas("game-pics");
  regoAtlas("cc-pics");

  CC_POS2(sprite, 640, 759);
  addAtlasItem("game-pics", sprite, E_LAYER_HUD);

  sprite = cx::reifySprite("TD_pumbtn.png");
  CC_POS2(sprite, 100, 755);
  addAtlasItem("game-pics", sprite, E_LAYER_HUD);

  sprite = cx::reifySprite("TD_bgbtn.png");
  CC_POS2(sprite, 500, 755);
  addAtlasItem("game-pics", sprite, E_LAYER_HUD);

  // create the cash label
  _cashLabel = cx::reifyBmfLabel("dft", FTOS(_cash));
  XCFG()->scaleBmfont(_cashLabel,40);
  CC_POS2(_cashLabel, 190, 755);
  addItem(_cashLabel, E_LAYER_HUD);

  // create the waves label
  _wavesLabel = cx::reifyBmfLabel("dft", "1/"+FTOS(_numWaves));
  XCFG()->scaleBmfont(_wavesLabel, 40);
  CC_POS2(_wavesLabel, 500, 755);
  addItem(_wavesLabel, E_LAYER_HUD);

  auto label = cx::reifyBmfLabel("dft", "Waves");
  XCFG()->scaleBmfont(label,40);
  CC_POS2(label, 650, 755);
  addItem(label, E_LAYER_HUD);

  // create the speed toggle
  auto menu_item_1 = c::MenuItemSprite::create(
      c::reifySprite("TD_2Xbtn.png"),
      c::reifySprite("TD_2Xbtn.png"));
  auto menu_item_2 = c::MenuItemSprite::create(
      c::reifySprite("TD_1Xbtn.png"),
      c::reifySprite("TD_1Xbtn.png"));
  auto speed_toggle = c::MenuItemToggle::createWithTarget(
      this, menu_selector(HUDLayer::onToggleSpeed), menu_item_1, menu_item_2, CC_NIL);
  CC_POS2(speed_toggle, 900, 755);
  addItem(cx::mkMenu(speed_toggle));

  // create & add the pause button
  auto button = cx::reifyMenuBtn("TD_pausebtn.png");
  button->setCallback([=](c::Ref*) {
      this->onPause();
      });
  CC_POS2(button, 1220, 755);
  addItem(cx::mkMenu(button));
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateLabels() {
  _wavesLabel->setString(FTOS(_currWaveIndex+1) + "/" + FTOS(_numWaves));
  _cashLabel->setString(FTOS(_cash));
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScore(int n) {
  _score += n;
  _scoreLabel->setString(FTOS(_score));
}


NS_END



