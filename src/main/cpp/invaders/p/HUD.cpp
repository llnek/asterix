// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2015, Ken Leung. All rights reserved.

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "2d/CCLabel.h"
#include "x2d/GameScene.h"
#include "x2d/XLives.h"
#include "HUD.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::showMenu(c::Ref* ) {
  MGMS()->sendMsg("/hud/showmenu");
}

//////////////////////////////////////////////////////////////////////////////
//
f::XLayer* HUDLayer::realize() {

  auto soff = CC_CSV(c::Integer, "S_OFF");
  auto tile = CC_CSV(c::Integer, "TILE");
  auto wb = cx::visBox();

  regoAtlas("game-pics");

  scoreLabel = cx::reifyBmfLabel("font.SmallTypeWriting", "0");
  scoreLabel->setAnchorPoint(cx::anchorTR());
  scoreLabel->setScale(XCFG()->getScale());
  scoreLabel->setPosition(wb.right - tile - soff, wb.top - tile);
  addItem(scoreLabel);

  this->lives= f::reifyRefType<f::XLives>();
  this->lives->realize("health.png", 3,
      tile + soff,
      wb.top - tile - soff);
  addItem(lives);

  auto b = cx::reifyMenuBtn("icon_menu.png");
  auto hh = cx::getHeight(b) * 0.5f;
  auto hw = cx::getWidth(b) * 0.5f;
  b->setTarget(this,
      CC_MENU_SELECTOR(HUDLayer::showMenu));
  auto menu = cx::mkMenu(b);
  menu->setPosition(wb.right - tile - hw, wb.bottom + tile  + hh);
  addItem(menu);

  return this;
}

void HUDLayer::reduceLives(int n) {

}

void HUDLayer::updateScore(int n) {

}

void HUDLayer::reset() {

}

NS_END(invaders)


