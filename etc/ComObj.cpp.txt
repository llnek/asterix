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

#include "ComObj.h"

NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
void ComObj::updatePosition(float x, float y) {
//TODO:
}

//////////////////////////////////////////////////////////////////////////////
//
void ComObj::hurt(int damage) {
  HP -= damage;
}

//////////////////////////////////////////////////////////////////////////////
//
void ComObj::inflate(not_null<c::Dictionary*> dict) {

  Poolable::inflate();
  HP = origHP;

  if (ENP(node)) {
    return;
  }

  auto scale= fusii::dictVal<c::Float>(dict, "scale");
  auto deg= fusii::dictVal<c::Float>(dict, "deg");
  auto x= fusii::dictVal<c::Float>(dict, "x");
  auto y= fusii::dictVal<c::Float>(dict, "y");

  if (NNP(x) && NNP(y)) {
    node->setPosition(x->getValue(), y->getValue());
  }
  if (NNP(deg)) {
    node->setRotation(deg->getValue());
  }
  if (NNP(scale)) {
    node->setScale(scale->getValue());
  }

  node->setVisible(true);
}

//////////////////////////////////////////////////////////////////////////////
//
void ComObj::inflate(float x, float y) {

  Poolable::inflate();
  HP = origHP;

  if (NNP(node)) {
    node->setPosition(x,y);
    node->setVisible(true);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void ComObj::inflate() {

  Poolable::inflate();
  HP = origHP;

  if (NNP(node)) {
    node->setVisible(true);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void ComObj::deflate(bool stop) {
  Poolable::deflate(stop);
  if (NNP(node)) {
    if (stop) {
      node->unscheduleAllCallbacks();
      node->stopAllActions();
    }
    node->setVisible(false);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
float ComObj::height() {
  return NNP(node) ? node->boundingBox().size.height : 0;
}

//////////////////////////////////////////////////////////////////////////////
//
float ComObj::width() {
  return NNP(node) ? node->boundingBox().size.width : 0;
}

//////////////////////////////////////////////////////////////////////////////
//
const c::Rect ComObj::bbox() {
  return NNP(node) ? node->boundingBox() : c::Rect(0,0,0,0);
}

//////////////////////////////////////////////////////////////////////////////
//
void ComObj::setPos(float x, float y) {
  if (NNP(node)) {
    node->setPosition(x,y);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void ComObj::hide() {
  if (NNP(node)) {
    node->setVisible(false);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void ComObj::show() {
  if (NNP(node)) {
    node->setVisible(true);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
const c::Vec2 ComObj::pos() {
  return NNP(node) ? node->getPosition() : c::Vec2(0,0);
}

//////////////////////////////////////////////////////////////////////////////
//
const c::Size ComObj::csize() {
  return NNP(node) ? CC_CSIZE(node) : c::Size(0,0);
}

//////////////////////////////////////////////////////////////////////////////
//
float ComObj::circum() {
  return NNP(node) ? CC_CSIZE(node).width : 0;
}

//////////////////////////////////////////////////////////////////////////////
//
float ComObj::radius() {
  return NNP(node) ? CC_CSIZE(node).width * 0.5f : 0;
}

//////////////////////////////////////////////////////////////////////////////
//
int ComObj::pid() {
  return NNP(node) ? node->getTag() : 0;
}

//////////////////////////////////////////////////////////////////////////////
//
void ComObj::init(c::Node *node, int health, int score) {
  this->origHP = health;
  this->node = node;
  this->HP = health;
  this->score = score;
  deflate();
}

//////////////////////////////////////////////////////////////////////////////
//
ComObj::ComObj(not_null<c::Node*> node, int health, int score) {
  init(node, health, score);
}

//////////////////////////////////////////////////////////////////////////////
//
ComObj::ComObj(not_null<c::Node*> node) {
  init(node,1,0);
}

//////////////////////////////////////////////////////////////////////////////
//
ComObj::ComObj() {
  init(nullptr,1,0);
}

//////////////////////////////////////////////////////////////////////////////
//
ComObj::~ComObj() {
  if (node) {
     node->removeFromParent();
  }
}





NS_END



