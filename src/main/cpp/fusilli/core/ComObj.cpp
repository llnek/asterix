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

#include "Primitives.h"
#include "ComObj.h"
NS_BEGIN(fusii)


//////////////////////////////////////////////////////////////////////////////
//
void ComObj::UpdatePosition(float x, float y) {
//TODO:
}

//////////////////////////////////////////////////////////////////////////////
//
void ComObj::Hurt(int damage) {
  health -= damage;
}

//////////////////////////////////////////////////////////////////////////////
//
void ComObj::Inflate(not_null<c::Dictionary*> dict) {

  health = origHealth;
  status=true;

  if (ENP(sprite)) {
    return;
  }

  auto scale= DictVal<c::Float>(dict,"scale");
  auto deg= DictVal<c::Float>(dict, "deg");
  auto x= DictVal<c::Float>(dict, "x");
  auto y= DictVal<c::Float>(dict, "y");

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
void ComObj::Inflate() {

  health = origHealth;
  status=true;

  if (NNP(sprite)) {
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
const c::Vec2 ComObj::Pos() {
  return NNP(sprite) ? sprite->getPosition() : c::Vec2(0,0);
}

//////////////////////////////////////////////////////////////////////////////
//
const c::Size ComObj::CSize() {
  return NNP(sprite) ? sprite->getContentSize() : c::Size(0,0);
}

//////////////////////////////////////////////////////////////////////////////
//
int ComObj::Pid() {
  return NNP(sprite) ? sprite->getTag() : 0;
}

//////////////////////////////////////////////////////////////////////////////
//
void ComObj::Init(c::Sprite* sprite, int health, int score) {
  this->origHealth = health;
  this->sprite = sprite;
  this->health = health;
  this->score = score;
  status=false;
}

//////////////////////////////////////////////////////////////////////////////
//
ComObj::ComObj(c::Sprite* sprite, int health, int score) {
  Init(sprite, health, score);
}

//////////////////////////////////////////////////////////////////////////////
//
ComObj::ComObj(c::Sprite* sprite) {
  Init(sprite,1,0);
}

//////////////////////////////////////////////////////////////////////////////
//
ComObj::ComObj() {
  Init(nullptr,1,0);
}

NS_END(fusii)

