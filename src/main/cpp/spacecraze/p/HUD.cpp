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

#include "core/CCSX.h"
#include "HUD.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(spacecraze)

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::decorate() {

  auto s= cx::reifySprite("sfscore");
  auto wb= cx::visBox();
  float y= wb.top*0.925f;
  float x;

  regoAtlas("game-pics");

  s->setPosition(wb.right *0.15f, y);
  addAtlasItem("game-pics", s);

  s= cx::reifySprite("sflives");
  s->setPosition(wb.right*0.7f, y);
  addAtlasItem("game-pics", s);

  x += s->getPositionX() + s->getContentSize().width;
  y += cx::calcSize("sflifei").height * 0.5;
  this->lives= f::reifyRefType<f::XLives>();
  this->lives->decorate("sflifei", 3, x, y);
  addItem(lives);

  y= wb.top*0.925f;

  scoreLabel = cx::reifyBmfLabel("sftext", "0");
  scoreLabel->setPosition(wb.right * 0.3f, y);
  addItem(scoreLabel);


  auto b= cx::reifyMenuBtn("sfpause");
  auto menu = cx::mkMenu(b);

  b->setPosition(wb.right*0.95f, y);
  b->setCallback([=](c::Ref*) {
      });
  addItem(menu);
}

//////////////////////////////////////////////////////////////////////////////
//
bool HUDLayer::reduceLives(int n) {
  lives->reduce(n);
  return lives->isDead();
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::updateScore(int n) {
  score += n;
  scoreLabel->setString(s::to_string(score));
}


NS_END


