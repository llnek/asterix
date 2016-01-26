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

  auto scaleBy = c::ScaleBy::create(duration, 3.0f, 3.0f);
  auto right = c::RotateBy::create(duration, 45.0f);
  auto seq = c::Sequence::createWithTwoActions(
      c::FadeOut::create(duration),
      c::CallFunc::create([=]() { this->deflate(); }));

  sprite2->setRotation( cx::randInt(360));
  sprite2->setOpacity(255.0f);
  sprite2->setPosition(x,y);
  sprite2->setScale(scale);
  sprite2->setVisible(true);
  sprite2->runAction(scaleBy->clone());
  sprite2->runAction(seq->clone());

  sprite->setOpacity(255.0f);
  sprite->setScale(scale);
  sprite->runAction(right);
  sprite->runAction(scaleBy);
  sprite->runAction(seq);

  ComObj::inflate(x,y);
}

//////////////////////////////////////////////////////////////////////////////
//
void Spark::deflate() {
  sprite2->setVisible(false);
  sprite2->stopAllActions();
  ComObj::deflate();
}

//////////////////////////////////////////////////////////////////////////////
//
void Explosion::inflate(float x, float y) {
  auto ac = c::AnimationCache::getInstance();
  auto ani= ac->getAnimation("Explosion");
  sprite->runAction(c::Sequence::create(
        c::Animate::create(ani),
        c::CallFunc::create([=]() {
          this->deflate();
        }), nullptr));
  ComObj::inflate(x, y);
}

//////////////////////////////////////////////////////////////////////////////
//
void HitEffect::inflate(float x, float y) {
  sprite->runAction(c::ScaleBy::create(0.3f, 2.0f, 2.0f));
  sprite->runAction(
      c::Sequence::create(c::FadeOut::create(0.3f),
      c::CallFunc::create([=]() { this->deflate();  } ), nullptr));
  sprite->setRotation( cx::randInt(360));
  sprite->setScale(scale);
  ComObj::inflate(x,y);
}


NS_END(terra)


