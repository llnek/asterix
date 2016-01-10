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

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "HUD.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(asteroids)

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScore(int n) {
  score += n;
  drawScore();
}

//////////////////////////////////////////////////////////////////////////////
//
bool HUDLayer::reduceLives(int n) {
  lives->reduce(n);
  return lives->isDead();
}
//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::resetAsNew() {
  score = 0;
  reset();
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::reset() {
  //replayBtn->setVisible(false);
  lives->resurrect();
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::drawScore() {
  scoreLabel->setString(s::to_string(score));
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::decorate() {
  auto soff = CC_CSV(c::Integer, "S_OFF");
  auto tile = CC_CSV(c::Integer, "TILE");
  auto wz= cx::visRect();
  auto wb = cx::visBox();

  regoAtlas( "game-pics");

  scoreLabel = cx::reifyBmfLabel("font.TinyBoxBB", "0");
  scoreLabel->setAnchorPoint(cx::anchorBR());
  scoreLabel->setScale( 12/72.0f);
  scoreLabel->setPosition( wz.size.width - tile - soff,
    wz.size.height - tile - soff - cx::getScaledHeight(scoreLabel));
  addItem(scoreLabel);

  this->lives= f::reifyRefType<f::XLives>();
  this->lives->decorate("rship_1.png", 3,
      tile + soff,
      wb.top - tile - soff, 0.5f);
  addItem(lives);

  auto b = cx::reifyMenuBtn("icon_menu.png");
  auto menu = cx::mkMenu(b);
  auto z2= cx::halfHW(b);

  b->setCallback([=](c::Ref*) { SENDMSG("/hud/showmenu"); });
  menu->setPosition(wb.right-tile-z2.width, wb.bottom + tile + z2.height);
  addItem(menu);
}


NS_END(asteroids)


