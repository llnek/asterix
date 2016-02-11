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
void HUDLayer::decorate() {
  auto wz=cx::visRect();
  auto wb=cx::visBox();

  regoAtlas("game-pics");

  scoreLabel= cx::reifyBmfLabel("dft", "0");
  scoreLabel->setAlignment(c::TextHAlignment::LEFT);
  scoreLabel->setWidth(wz.size.width * 0.5f);
  scoreLabel->setAnchorPoint(cx::anchorL());
  scoreLabel->setPosition(wb.right * 0.48f, wb.top * 0.95f);
  addItem(scoreLabel,kBackground);

  auto btn = cx::reifyMenuBtn("btn_pause_off.png");
  auto mnu= cx::mkMenu(btn);
  btn->setPosition(wb.right * 0.06f, wb.top* 0.95f);
  CC_HIDE(mnu);
  addItem(mnu, kBackground);

  paused = cx::reifySprite("label_paused.png");
  paused->setPosition(wb.cx, wb.top * 0.55f);
  CC_HIDE(paused);
  addAtlasItem("game-pics",paused, kForeground);

  score=0;
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScore(int v) {
  score += v;
  scoreLabel->setString(s::to_string(score));
}

NS_END



