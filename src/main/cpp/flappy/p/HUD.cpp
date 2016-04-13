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
#include "n/lib.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(flappy)

//////////////////////////////////////////////////////////////////////////
//
void HUDLayer::decoUI() {

  auto wb= cx::visBox();
  _score=0;

  _scoreLabel= cx::reifyLabel("dft", 120, "0");
  _scoreLabel->setPosition(wb.cx, wb.top * 0.875);
  addItem(_scoreLabel, E_LAYER_HUD);

  // create the tutorial sprite and add it to the batch node
  _tutorialSprite = cx::reifySprite("dhtap");
  _tutorialSprite->setPosition(wb.cx, wb.cy);
  addItem(_tutorialSprite,E_LAYER_HUD);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::getReady() {
  if (_tutorialSprite) {
            // fade it out and then remove it
    _tutorialSprite->runAction(
        c::Sequence::create(
          c::FadeOut::create(0.25),
          c::RemoveSelf::create(true),
          CC_NIL));
    _tutorialSprite = CC_NIL;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScore(int n) {
  _score += n;
  _scoreLabel->setString(FTOS(_score));
  // run a simple action so the user knows the score is being added
        // use the ease functions to create a heart beat effect
  _scoreLabel->runAction(
      c::Sequence::create(
        c::EaseSineIn::create(
          c::ScaleTo::create(0.125, 1.2)),
        c::EaseSineOut::create(c::ScaleTo::create(0.125, 1)),
        CC_NIL));
}


NS_END



