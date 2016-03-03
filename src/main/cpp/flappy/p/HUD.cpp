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
#include "n/lib.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(flappy)

//////////////////////////////////////////////////////////////////////////
//
void HUDLayer::decoUI() {

  auto wb= cx::visBox();
  score=0;

  scoreLabel= cx::reifyLabel("dft", 120, "0");
  scoreLabel->setPosition(wb.cx, wb.top * 0.875);
  addItem(scoreLabel, E_LAYER_HUD);

  // create the tutorial sprite and add it to the batch node
  tutorialSprite = cx::reifySprite("dhtap");
  tutorialSprite->setPosition(wb.cx, wb.cy);
    addItem(tutorialSprite,E_LAYER_HUD);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::getReady() {
  if (tutorialSprite) {
            // fade it out and then remove it
    tutorialSprite->runAction(
        c::Sequence::create(
          c::FadeOut::create(0.25),
          c::RemoveSelf::create(true),
          CC_NIL));
    tutorialSprite = CC_NIL;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScore(int n) {
  score += n;
  scoreLabel->setString(s::to_string(score));
  // run a simple action so the user knows the score is being added
        // use the ease functions to create a heart beat effect
  scoreLabel->runAction(
      c::Sequence::create(
        c::EaseSineIn::create(
          c::ScaleTo::create(0.125, 1.2)),
        c::EaseSineOut::create(c::ScaleTo::create(0.125, 1)),
        CC_NIL));
}


NS_END



