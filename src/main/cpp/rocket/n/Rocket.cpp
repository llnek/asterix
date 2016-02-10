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
  MGML()->addAtlasItem("game-pics", s, kForeground, kSpriteRocket);
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
/*
	Point position = this->getPosition();

	if (_rotationOrientation == ROTATE_NONE) {

		position.x += _vector.x * dt;
		position.y += _vector.y * dt;

	} else {

		//rotate point around a pivot by a certain amount (rotation angle)
        float angle = _angularSpeed * dt;
        Point rotatedPoint = position.rotateByAngle(_pivot, angle);
        position.x = rotatedPoint.x;
        position.y = rotatedPoint.y;

        float rotatedAngle;

        Point diff = position;
        diff.subtract(_pivot);
        Point clockwise = diff.getRPerp();

         if (_rotationOrientation == ROTATE_COUNTER) {
             rotatedAngle = atan2 (-1 * clockwise.y, -1 * clockwise.x);
         } else {
             rotatedAngle = atan2 (clockwise.y, clockwise.x);
         }

         //update rocket vector
         _vector.x = _speed * cos (rotatedAngle);
         _vector.y = _speed * sin (rotatedAngle);

         this->setRotationFromVector();

         //wrap rotation values to 0-360 degrees
         if (this->getRotation() > 0) {
             this->setRotation( fmodf(this->getRotation(), 360.0f) );
         } else {
             this->setRotation( fmodf(this->getRotation(), -360.0f) );
         }
	}

    if (_targetRotation > this->getRotation() + 180) {
        _targetRotation -= 360;
    }
    if (_targetRotation < this->getRotation() - 180) {
        _targetRotation += 360;
    }

    this->setPosition(position);

    _dr = _targetRotation - this->getRotation() ;
    _ar = _dr * _rotationSpring;
    _vr += _ar ;
    _vr *= _rotationDamping;
    float rotationNow = this->getRotation();
    rotationNow += _vr;
    this->setRotation(rotationNow);
*/
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
/*
    Size screenSize = Director::getInstance()->getWinSize();


    if (_position.x > screenSize.width - _radius) {
        _position.x =  screenSize.width - _radius;
        _rotationOrientation = ROTATE_NONE;
        _vector = Vec2(this->getVector().x * -1, this->getVector().y);
        this->setRotationFromVector();
        return true;
    }

    if (_position.x < _radius) {
        _position.x =  _radius;
       	_rotationOrientation =  ROTATE_NONE;
        _vector =  Vec2 (this->getVector().x * -1, this->getVector().y);
        this->setRotationFromVector();
        return true;

    }

    if (_position.y < _radius) {
        _position.y = _radius;
       	_rotationOrientation = ROTATE_NONE;
        _vector =  Vec2 (this->getVector().x, this->getVector().y * -1);
        this->setRotationFromVector();
        return true;
    }

    if (_position.y > screenSize.height - _radius) {
        _position.y =  screenSize.height - _radius;
       	_rotationOrientation = ROTATE_NONE;
        _vector =  Vec2 (this->getVector().x, this->getVector().y * -1);
        this->setRotationFromVector();
        return true;
    }
*/
    return false;
}


NS_END


