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
NS_BEGIN(blast)

//////////////////////////////////////////////////////////////////////////
//
void HUDLayer::decoUI() {

  auto wb= cx::visBox();
  _score=0;

  _scoreLabel= cx::reifyBmfLabel("dft", "Score: 0");
  _scoreLabel->setAnchorPoint(cx::anchorBL());
  _scoreLabel->setPosition(wb.right * 0.1, wb.top * 0.9);

  addItem(_scoreLabel, E_LAYER_HUD);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::flashCombo(int n, const c::Vec2 &pos) {

  auto msg= "X"+ FTOS(n);

  auto label = cx::reifyBmfLabel("dft", msg);
  label->setPosition(pos.x, pos.y + CC_ZH(CC_CSIZE(label)));
  label->setScale(0.6);

  // animate it to move upwards then remove it
  label->runAction(
      c::Sequence::create(
        c::MoveBy::create(1, c::Vec2(0, 50)),
        c::DelayTime::create(0.5),
        c::RemoveSelf::create(true),
        CC_NIL));
  addItem(label, E_LAYER_HUD);

}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScore(int n) {
  _score += n;
  _scoreLabel->setString(FTOS(_score));
}


NS_END



