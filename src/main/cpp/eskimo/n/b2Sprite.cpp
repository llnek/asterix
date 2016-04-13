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

#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"
#include "b2Sprite.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(eskimo)

//////////////////////////////////////////////////////////////////////////////
//
void b2Sprite::setSpritePosition(const c::Vec2 &pos) {

  setPosition(pos);

  if (_body) {
    _body->SetTransform(b2Vec2(
                          pos.x / PTM_RATIO,
                          pos.y/ PTM_RATIO),
                          _body->GetAngle());
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void b2Sprite::update() {
  if (_body) {
    setPositionX(_body->GetPosition().x * PTM_RATIO);
    setPositionY(_body->GetPosition().y * PTM_RATIO);
    setRotation(CC_RADIANS_TO_DEGREES(-1 * _body->GetAngle()));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void b2Sprite::hide() {
  if (_body) {
    _body->SetLinearVelocity(b2Vec2_zero);
    _body->SetAngularVelocity(0);
    _body->SetTransform(b2Vec2_zero, 0.0);
    _body->SetAwake(false);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void b2Sprite::reset() {
}


NS_END


