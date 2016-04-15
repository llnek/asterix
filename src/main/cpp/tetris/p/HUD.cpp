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

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::decoUI() {

  auto wz = cx::visSize();
  auto wb = cx::visBox();
  float t=0;

  regoAtlas("game-pics");
  _score=0;

  _scoreLabel = cx::reifyBmfLabel("score", "0");
  _scoreLabel->setAnchorPoint(cx::anchorTR());
  XCFG()->scaleBmfont(_scoreLabel,24);
  t= CC_CHT(_scoreLabel)/2;
  CC_POS2(_scoreLabel, wb.right - t, wb.top - t);
  addItem(_scoreLabel);

  _status= cx::reifyBmfLabel("dft");
  XCFG()->scaleBmfont(_status, 16);
  CC_POS2(_status, wb.cx * 1.5, wb.cy);
  CC_HIDE(_status);
  addItem(_status);

  auto b = cx::reifyMenuBtn("icon_menu.png");
  auto w= CC_CWH(b);

  b->setCallback([](c::Ref*) { SENDMSG("/hud/showmenu"); });
  b->setAnchorPoint(cx::anchorBR());
  CC_POS2(b, wb.right - w/3, wb.bottom + w/3);
  addItem(cx::mkMenu(b));
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::endGame() {
  drawStatusText(gets("gameover"));
  CC_SHOW(_status);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::drawStatusText(const sstr &msg) {
  _status->setString( msg);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScore(int n) {
  _score += n;
  _scoreLabel->setString(FTOS(_score));
}


NS_END


