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
#include "MMenu.h"
#include "HUD.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////
//
void HUDLayer::decorate() {
  auto soff = CC_CSV(c::Float, "S_OFF");
  auto tile = CC_CSV(c::Float, "TILE");
  auto wz = cx::visRect();
  auto wb= cx::visBox();

  regoAtlas("game-pics");
  incIndexZ();

  scoreLabel= cx::reifyBmfLabel("TinyBoxBB", "0");
  scoreLabel->setAnchorPoint(cx::anchorBR());
  scoreLabel->setScale(12.0f/72.0f);
  scoreLabel->setPosition( wz.size.width - tile - soff,
      wz.size.height - tile - soff - cx::getHeight(scoreLabel));
  addItem(scoreLabel);

  lives = f::reifyRefType<f::XLives>();
  lives->decorate("ship01.png",3,
      tile +soff,
      wz.size.height - tile - soff, 0.4f);
  addItem(lives);

  auto b = cx::reifyMenuBtn("icon_menu.png");
  auto hh = cx::getHeight(b) * 0.5f;
  auto hw = cx::getWidth(b) * 0.5f;
  auto menu = cx::mkMenu(b);

  //b->setColor(this->color);
  b->setCallback(
      [=](c::Ref*) { SENDMSG("/hud/showmenu"); });

  menu->setPosition( wb.right - tile - hw, wb.bottom + tile  + hh);
  addItem(menu);

  lives->resurrect();
  score=0;
}

//////////////////////////////////////////////////////////////////////////
//
bool HUDLayer::reduceLives(int n) {
  lives->reduce(n);
  return lives->isDead();
}

//////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScore(int n) {
  score += n;
  scoreLabel->setString(s::to_string(score));
}


NS_END(terra)

