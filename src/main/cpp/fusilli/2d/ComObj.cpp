// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2015, Ken Leung. All rights reserved.

#include "ComObj.h"
NS_BEGIN(fusilli)


//////////////////////////////////////////////////////////////////////////////
//
void ComObj::Hurt(int damage) {
  health -= damage;
}

//////////////////////////////////////////////////////////////////////////////
//
void ComObj::Inflate(cc::Dictionary* dict) {

  health = origHealth;
  status=true;

  if (ENP(sprite)) {
    return;
  }

  auto scale= SCAST(cc::Float*, dict->objectForKey("scale"));
  auto deg= SCAST(cc::Float*, dict->objectForKey("deg"));
  auto x= SCAST(cc::Float*, dict->objectForKey("x"));
  auto y= SCAST(cc::Float*, dict->objectForKey("y"));

  if (NNP(x) && NNP(y)) {
    sprite->setPosition(x->getValue(), y->getValue());
  }
  if (NNP(deg)) {
    sprite->setRotation(deg->getValue());
  }
  if (NNP(scale)) {
    sprite->setScale(scale->getValue());
  }

  sprite->setVisible(true);
}

//////////////////////////////////////////////////////////////////////////////
//
void ComObj::Inflate(float x, float y) {

  health = origHealth;
  status=true;

  if (NNP(sprite)) {
    sprite->setPosition(x,y);
    sprite->setVisible(true);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void ComObj::Deflate() {
  if (NNP(sprite)) {
    sprite->unscheduleAllCallbacks();
    sprite->stopAllActions();
    sprite->setVisible(false);
  }
  status=false;
}

//////////////////////////////////////////////////////////////////////////////
//
float ComObj::Height() {
  return NNP(sprite) ? sprite->getContentSize().height : 0;
}

//////////////////////////////////////////////////////////////////////////////
//
float ComObj::Width() {
  return NNP(sprite) ? sprite->getContentSize().width : 0;
}

//////////////////////////////////////////////////////////////////////////////
//
void ComObj::SetPos(float x, float y) {
  if (NNP(sprite)) {
    sprite->setPosition(x,y);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
const cc::Vec2 ComObj::Pos() {
  return NNP(sprite) ? sprite->getPosition() : cc::Vec2(0,0);
}

//////////////////////////////////////////////////////////////////////////////
//
const cc::Size ComObj::CSize() {
  return NNP(sprite) ? sprite->getContentSize() : cc::Size(0,0);
}

//////////////////////////////////////////////////////////////////////////////
//
int ComObj::Pid() {
  return NNP(sprite) ? sprite->getTag() : 0;
}

//////////////////////////////////////////////////////////////////////////////
//
ComObj::ComObj(cc::Sprite* sprite, int health, int score) {
  this->origHealth = health;
  this->sprite = sprite;
  this->health = health;
  this->score = score;
  status=false;
}

//////////////////////////////////////////////////////////////////////////////
//
ComObj::ComObj(cc::Sprite* sprite)
  : ComObj(sprite,1,0) {
}


NS_END(fusilli)

