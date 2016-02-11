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
#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Rocket.h"
#include "C.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(rocket)

//////////////////////////////////////////////////////////////////////////////
//
Rocket::Rocket(not_null<c::Sprite*> s)
  : ComObj(s) {

  targetRotation=0;
  dr=0;
  ar=0;
  vr=0;
  rotationSpring= 0.1f;
  rotationDamping= 0.8f;
  pivot= c::Vec2(-1,-1);
  rotationOrientation= ROTATE_NONE;
}

//////////////////////////////////////////////////////////////////////////////
//
Rocket* Rocket::create() {
  auto s = cx::reifySprite("rocket.png");
  auto r= mc_new1(Rocket,s);
  r->reset();
  return r;
}

//////////////////////////////////////////////////////////////////////////////
//
void Rocket::reset() {
  rotationOrientation = ROTATE_NONE;
  targetRotation = -90;
  pivot = c::Vec2(-1,1);
  speed.x = 80;
  speed.y = 80;
  sprite->setRotation(-90);
  auto angle = CC_DEGREES_TO_RADIANS(sprite->getRotation());
  vel = c::Vec2(speed.x * cos(angle), -speed.y * sin(angle));
}

//////////////////////////////////////////////////////////////////////////////
//
void Rocket::update (float dt) {

  auto position = sprite->getPosition();

  if (rotationOrientation == ROTATE_NONE) {
    position.x += vel.x * dt;
    position.y += vel.y * dt;
  } else {
    //rotate point around a pivot by a certain amount (rotation angle)
    auto angle = angularSpeed * dt;
    auto rotatedPoint = position.rotateByAngle(pivot, angle);
    position.x = rotatedPoint.x;
    position.y = rotatedPoint.y;

    auto diff = position;
    float rotatedAngle;
    diff.subtract(pivot);
    auto clockwise = diff.getRPerp();

    if (rotationOrientation == ROTATE_COUNTER) {
      rotatedAngle = atan2 (-1 * clockwise.y, -1 * clockwise.x);
    } else {
      rotatedAngle = atan2 (clockwise.y, clockwise.x);
    }

    //update rocket vector
    vel.x = speed.x * cos (rotatedAngle);
    vel.y = speed.y * sin (rotatedAngle);

    setRotationFromVector();

    //wrap rotation values to 0-360 degrees
    if (sprite->getRotation() > 0) {
      sprite->setRotation( fmodf(sprite->getRotation(), 360.0f) );
    } else {
      sprite->setRotation( fmodf(sprite->getRotation(), -360.0f) );
    }
  }

  if (targetRotation > sprite->getRotation() + 180) {
    targetRotation -= 360;
  }
  if (targetRotation < sprite->getRotation() - 180) {
    targetRotation += 360;
  }

  sprite->setPosition(position);

  dr = targetRotation - sprite->getRotation() ;
  ar = dr * rotationSpring;
  vr += ar ;
  vr *= rotationDamping;

  auto rotationNow = sprite->getRotation();
  rotationNow += vr;

  sprite->setRotation(rotationNow);
}

//////////////////////////////////////////////////////////////////////////////
//
void Rocket::select(bool flag) {
  auto f= flag ? "rocket_on.png" : "rocket.png";
  sprite->setDisplayFrame(cx::getSpriteFrame(f));
}

//////////////////////////////////////////////////////////////////////////////
//
bool Rocket::collidedWithSides() {

  auto position= sprite->getPosition();
  auto r= this->radius();
  auto wz = cx::visRect();
  auto wb=cx::visBox();

  if (position.x > wb.right - r) {
    sprite->setPositionX(wb.right-r);
    rotationOrientation = ROTATE_NONE;
    vel = c::Vec2(vel.x * -1, vel.y);
    this->setRotationFromVector();
    return true;
  }

  if (position.x < r) {
    sprite->setPositionX(r);
    rotationOrientation = ROTATE_NONE;
    vel =  c::Vec2 (vel.x * -1, vel.y);
    this->setRotationFromVector();
    return true;
  }

  if (position.y < r) {
    sprite->setPositionY(r);
    rotationOrientation = ROTATE_NONE;
    vel =  c::Vec2 (vel.x, vel.y * -1);
    this->setRotationFromVector();
    return true;
  }

  if (position.y > wb.top - r) {
    sprite->setPositionY(wb.top - r);
    rotationOrientation = ROTATE_NONE;
    vel =  c::Vec2 (vel.x, vel.y * -1);
    this->setRotationFromVector();
    return true;
  }

  return false;
}


NS_END


