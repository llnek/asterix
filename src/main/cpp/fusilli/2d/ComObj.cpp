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

//////////////////////////////////////////////////////////////////////////////
//
void ComObj::Hurt(float damage) {
  health -= damage;
}

//////////////////////////////////////////////////////////////////////////////
//
void ComObj::Inflate(Dictionary* options) {

  health = origHealth;
  status=true;

  if (ENP(sprite)) {
    return;
  }

  auto scale= SCAST(Float*, dict->objectForKey("scale"));
  auto deg= SCAST(Float*, dict->objectForKey("deg"));
  auto x= SCAST(Float*, dict->objectForKey("x"));
  auto y= SCAST(Float*, dict->objectForKey("y"));

  if (NNP(x) && NNP(y)) {
    sprite->setRotation(x->getValue(), y->getValue());
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
void ComObj::Deflate() {
  if (NNP(sprite)) {
    sprite->unscheduleAllCallbacks();
    sprite->stopAllActions();
    sprite->setVisible(false);
  }
  status=false;
}

float ComObj::Height() {
  return NNP(sprite) ? sprite->getContentSize().height : 0;
  }
}

float ComObj::Width() {
  return NNP(sprite) ? sprite->.getContentSize().width : 0;
}

void ComObj::SetPos(float x, float y) {
  if (NNP(sprite)) {
    sprite->setPosition(x,y);
  }
}

const Vec2 pos() {
  return NNP(sprite) ? sprite->getPosition() : Vec2(0,0);
}

const Size ComObj::Size() {
  return NNP(sprite) ? sprite->getContentSize() : Size(0,0);
}

const string ComObj::RTTI() {
  return _name;
}

void ComObj::Rego(const string& n) {
  _name = n;
}

int ComObj::Pid() {
  return NNP(sprite) ? sprite->getTag() : 0;
}


ComObj::ComObj(Sprite* sprite, float health, float score) {
  this->origHealth = health;
  this->sprite = sprite;
  this->health = health;
  this->value = score;
  status=false;
}

ComObj::ComObj(Sprite* sprite)
  : ComObj(sprite,1,0) {
}

