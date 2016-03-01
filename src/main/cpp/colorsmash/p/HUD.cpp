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

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(colorsmash)

//////////////////////////////////////////////////////////////////////////
//
void HUDLayer::decoUI() {

  auto wb= cx::visBox();

  time = 60;
  score=0;

  scoreLabel = cx::reifyLabel("dft", 18, "");
  scoreLabel->setPosition(wb.right * 0.33, wb.top * 0.875);
  addItem(scoreLabel);

  timeLabel = cx::reifyLabel("dft", 18, "");
  timeLabel->setPosition(wb.right * 0.66, wb.top * 0.875);
  addItem(timeLabel);

  auto pause= cx::createMenuBtn("pause");
  pause->setCallback([=](c::Ref*){
  });
  pause->setPosition(wb.right * 0.9, wb.top * 0.95);
  auto menu= cx::mkMenu(pause);
  addItem(menu,1);

  updateTimer(0);
  updateScore(0);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateTimer(int n) {
  time += n;
  timeLabel->setString("Time: " + s::to_string(time));
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScore(int n) {
  score += n;
  scoreLabel->setString("Score: " + s::to_string(score));
  scoreLabel->runAction(
      c::Sequence::create(
        c::EaseSineIn::create( c::ScaleTo::create(0.125, 1.1)),
        c::EaseSineOut::create(c::ScaleTo::create(0.125, 1)),
        CC_NIL));
}


NS_END



