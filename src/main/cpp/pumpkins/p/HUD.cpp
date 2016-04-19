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
  auto sp = cx::reifySprite("TD_hudbg.png");
  XCFG()->fit(sp);
  CC_POS2(sp, 640, 759);
  addItem(sp, E_LAYER_HUD);

  sp = cx::reifySprite("TD_pumbtn.png");
  XCFG()->fit(sp);
  CC_POS2(sp, 100, 755);
  addItem(sp, E_LAYER_HUD);

  sp= cx::reifySprite("TD_bgbtn.png");
  XCFG()->fit(sp);
  CC_POS2(sp, 500, 755);
  addItem(sp, E_LAYER_HUD);

  // create the cash label
  _cashLabel = cx::reifyBmfLabel("dft", "0");
  XCFG()->scaleBmfont(_cashLabel, 40);
  CC_POS2(_cashLabel, 190, 755);
  addItem(_cashLabel, E_LAYER_HUD);

  // create the waves label
  _wavesLabel = cx::reifyBmfLabel("dft", "1/0");
  XCFG()->scaleBmfont(_wavesLabel, 40);
  CC_POS2(_wavesLabel, 500, 755);
  addItem(_wavesLabel, E_LAYER_HUD);

  auto label = cx::reifyBmfLabel("dft", "Waves");
  XCFG()->scaleBmfont(label, 40);
  CC_POS2(label, 650, 755);
  addItem(label, E_LAYER_HUD);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::createMenu() {
  _hudMenu = c::Menu::create();
  _hudMenu->setAnchorPoint(CC_ZPT);
  _hudMenu->setPosition(CC_ZPT);
  addItem(_hudMenu, E_LAYER_HUD);

  // create the speed toggle
  auto m1 = cx::reifyMenuBtn("TD_2Xbtn.png");
  XCFG()->fit(m1);
  auto m2 = cx::reifyMenuBtn("TD_1Xbtn.png");
  XCFG()->fit(m2);
  c::Vector<c::MenuItem*> items;
  items.pushBack(m1);
  items.pushBack(m2);
  auto toggle = c::MenuItemToggle::createWithCallback(
      [=](c::Ref*) {
        SENDMSG("/game/hud/togglespeed");
      }, items);
  CC_POS2(toggle, 900, 755);
  _hudMenu->addChild(toggle);

  // create & add the pause button
  auto button = cx::reifyMenuBtn("TD_pausebtn.png");
  button->setCallback([=](c::Ref*) {
      SENDMSG("/game/hud/pause");
      });
  CC_POS2(button, 1220, 755);
  _hudMenu->addChild(button);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::toggleMenu(bool b) {
  _hudMenu->setEnabled(b);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateLabels(not_null<GVars*> ss) {
  _wavesLabel->setString(FTOS(ss->currWaveIndex+1) + "/" + FTOS(ss->numWaves));
  _cashLabel->setString(FTOS(ss->cash));
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScore(int n) {
  _score += n;
  _scoreLabel->setString(FTOS(_score));
}


NS_END



