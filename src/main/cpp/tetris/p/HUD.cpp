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

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::decorate() {

  auto tile = CC_CSV(c::Float, "TILE");
  auto gz= XCFG()->gameSize();
  auto wz = cx::visRect();
  auto wb = cx::visBox();

  regoAtlas("game-pics");
  score=0;

  scoreLabel = cx::reifyBmfLabel("SmallTypeWriting", "0");
  scoreLabel->setAnchorPoint(cx::anchorTR());
  scoreLabel->setScale(XCFG()->getScale());
  scoreLabel->setPosition(
      wb.right - (tile * wz.size.width/gz.width),
      wb.top - (wz.size.height/gz.height * tile));
  addItem(scoreLabel);

  status= cx::reifyBmfLabel("CoffeeBuzzed");
  status->setScale( XCFG()->getScale() * 0.5f );
  status->setPosition(wb.cx * 1.5f, wb.cy);
  status->setVisible(false);
  addItem(status);

  auto b = cx::reifyMenuBtn("icon_menu.png");
  auto hh = cx::getHeight(b) * 0.5f;
  auto hw = cx::getWidth(b) * 0.5f;
  auto menu = cx::mkMenu(b);

  b->setCallback([](c::Ref*) { SENDMSG("/hud/showmenu"); });
  menu->setPosition(wb.right - tile - hw, wb.bottom + tile  + hh);
  addItem(menu);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::endGame() {
  drawStatusText(gets("gameover"));
  status->setVisible(true);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::drawStatusText(const sstr &msg) {
  status->setString( msg);
}


//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScore(int n) {
  score += n;
  scoreLabel->setString(s::to_string(score));
}


NS_END(tetris)


