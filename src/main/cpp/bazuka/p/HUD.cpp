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
#include "MMenu.h"
NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(bazuka)

//////////////////////////////////////////////////////////////////////////
//
void HUDLayer::decoUI() {

  auto wb= cx::visBox();

  scoreLabel = cx::reifyBmfLabel("pixel", "0");
  scoreLabel->setPosition(wb.cx, wb.top * 0.9);
  scoreLabel->setScale(0.5);
  addItem(scoreLabel, 10);

  auto pause = cx::reifyMenuBtn("pause.png");
  auto mnu = cx::mkMenu(pause);
  auto sz= CC_CSIZE(pause);
  pause->setPosition(wb.right- HWZ(sz), wb.top - HHZ(sz));
  pause->setCallback([=](c::Ref*){
    cx::sfxPlay("pop");
    cx::pushEx(MMenu::reify());
  });
  addItem(mnu);

  score=0;
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScore(int n) {
  score += n;
  scoreLabel->setString(s::to_string(score));
}


NS_END



