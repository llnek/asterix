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

#include "C.h"

NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
//
void Spark::inflate(float x, float y) {

  auto scaleBy = c::ScaleBy::create(duration, 3.0, 3.0);
  auto right = c::RotateBy::create(duration, 45.0);
  auto seq = c::Sequence::createWithTwoActions(
      c::FadeOut::create(duration),
      c::CallFunc::create([=]() { this->deflate(); }));

  sprite2->setRotation( cx::randInt(360));
  sprite2->setOpacity(255);
  sprite2->setPosition(x,y);
  sprite2->setScale(scale);
  CC_SHOW(sprite2);
  sprite2->runAction(scaleBy->clone());
  sprite2->runAction(seq->clone());

  node->setOpacity(255);
  node->setScale(scale);
  node->runAction(right);
  node->runAction(scaleBy);
  node->runAction(seq);

  f::CPixie::inflate(x,y);
}

//////////////////////////////////////////////////////////////////////////////
//
void Spark::deflate() {
  CC_HIDE(sprite2);
  sprite2->stopAllActions();
  f::CPixie::deflate();
}

//////////////////////////////////////////////////////////////////////////////
//
void Explosion::inflate(float x, float y) {
  auto ani= CC_ACAC()->getAnimation("Explosion");
  node->runAction(c::Sequence::create(
        c::Animate::create(ani),
        c::CallFunc::create([=]() {
          this->deflate();
        }), CC_NIL));
  f::CPixie::inflate(x, y);
}

//////////////////////////////////////////////////////////////////////////////
//
void HitEffect::inflate(float x, float y) {
  node->runAction(c::ScaleBy::create(0.3, 2, 2));
  node->runAction(
      c::Sequence::create(
        c::FadeOut::create(0.3),
        c::CallFunc::create(
          [=]() { this->deflate(); }),
        CC_NIL));
  node->setRotation(cx::randInt(360));
  node->setScale(scale);
  f::CPixie::inflate(x,y);
}


NS_END


