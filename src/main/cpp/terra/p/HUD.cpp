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
#include "MMenu.h"
#include "HUD.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////
//
void HUDLayer::decoUI() {
  auto wb= cx::visBox();
  float t=0;

  regoAtlas("game-pics");
  regoAtlas("cc-pics");

  _scoreLabel= cx::reifyBmfLabel("score", "0");
  _scoreLabel->setAnchorPoint(cx::anchorTR());
  XCFG()->scaleLabel(_scoreLabel,24);
  t= CC_CHT(_scoreLabel)/GOLDEN_RATIO;
  CC_POS2(_scoreLabel, wb.right - t, wb.top - t);
  addItem(_scoreLabel);

  _lives = f::reifyRefType<f::XLives>();
  _lives->initLives("ship01.png",3,
      wb.left + t,
      wb.top - t,
      0.4);
  addItem(_lives);

  auto b = cx::reifyMenuBtn("icon_menu.png");
  t= CC_CWH(b)/GOLDEN_RATIO;
  b->setCallback(
      [=](c::Ref*) { SENDMSG("/hud/showmenu"); });
  CC_POS2(b, wb.right - t, wb.bottom + t);
  //b->setColor(this->color);
  addItem(cx::mkMenu(b));

  _lives->resurrect();
  _score=0;
}

//////////////////////////////////////////////////////////////////////////
//
bool HUDLayer::reduceLives(int n) {
  return _lives->reduce(n)->isDead();
}

//////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScore(int n) {
  _score += n;
  _scoreLabel->setString(FTOS(_score));
}


NS_END

