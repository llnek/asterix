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
NS_BEGIN(rocket)

//////////////////////////////////////////////////////////////////////////
//
void HUDLayer::decoUI() {

  auto wz=cx::visSize();
  auto wb=cx::visBox();

  regoAtlas("game-pics");
  regoAtlas("cc-pics");

  _scoreLabel= cx::reifyBmfLabel("dft", "0");
  _scoreLabel->setAlignment(c::TextHAlignment::LEFT);
  _scoreLabel->setAnchorPoint(cx::anchorL());
  _scoreLabel->setWidth(HWZ(wz));
  CC_POS2(_scoreLabel, wb.right * 0.48, wb.top * 0.95);
  addItem(_scoreLabel,kBackground);

  auto btn = cx::reifyMenuBtn("btn_pause_off.png");
  auto mnu= cx::mkMenu(btn);
  CC_POS2(btn, wb.right * 0.06, wb.top* 0.95);
  CC_HIDE(mnu);
  addItem(mnu, kBackground);

  _paused = cx::reifySprite("label_paused.png");
  CC_POS2(_paused, wb.cx, wb.top * 0.55);
  CC_HIDE(_paused);
  addAtlasItem("game-pics", _paused, kForeground);

  _score=0;
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScore(int v) {
  _score += v;
  _scoreLabel->setString(FTOS(_score));
}


NS_END



