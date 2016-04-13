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

#include "core/CCSX.h"
#include "HUD.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(spacecraze)

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::decoUI() {

  auto lfsz = cx::calcSize("sflifei");
  auto s= cx::reifySprite("sfscore");
  auto wb= cx::visBox();
  auto y= wb.top*0.925;
  float x=0;

  regoAtlas("game-pics");
  regoAtlas("cc-pics");

  CC_POS2(s, wb.right * 0.15, y);
  addAtlasItem("game-pics", s);

  s= cx::reifySprite("sflives");
  CC_POS2(s, wb.right*0.7, y);
  addAtlasItem("game-pics", s);

  x += s->getPositionX() + CC_ZW(CC_CSIZE(s));
  y += HHZ(lfsz);

  _lives= f::reifyRefType<f::XLives>();
  _lives->initLives("sflifei", 3, x, y);
  addItem(_lives);

  y= wb.top*0.925;

  _scoreLabel= cx::reifyBmfLabel("score", "0");
  CC_POS2(_scoreLabel, wb.right * 0.3, y);
  addItem(_scoreLabel);

  auto b= cx::reifyMenuBtn("sfpause");
  CC_POS2(b, wb.right*0.95, y);
  b->setCallback([=](c::Ref*) {
      });
  addItem(cx::mkMenu(b));
}

//////////////////////////////////////////////////////////////////////////////
//
bool HUDLayer::reduceLives(int n) {
  return _lives->reduce(n)->isDead();
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScore(int n) {
  _score += n;
  _scoreLabel->setString(FTOS(_score));
}


NS_END


