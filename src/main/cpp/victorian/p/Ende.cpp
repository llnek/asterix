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

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Splash.h"
#include "Game.h"
#include "Ende.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(victorian)

//////////////////////////////////////////////////////////////////////////////
//
void Ende::decoUI() {
  auto lbl= cx::reifyLabel("text", 60, "Game Over!");
  auto wb= cx::visBox();

  lbl->setPosition(wb.cx, wb.top * 0.8);
  addItem(lbl);

  _replayBtn = cx::reifySprite("label_tryagain.png");
  _replayBtn->setPosition(wb.cx, wb.cy);
  addItem(_replayBtn );

  enableListeners();
}

//////////////////////////////////////////////////////////////////////////////
//
bool Ende::onTouchStart(c::Touch *t) {
  return onMouseStart(t->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Ende::onMouseStart(const CCT_PT &tap) {
  if (cx::isTapped(_replayBtn, tap)) {
    auto x= MGMS()->ejectCtx();
    cx::runEx(Game::reify(x));
  }
  return false;
}

NS_END



