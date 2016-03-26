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

#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"
#include "Player.h"

#define P_ACCELERATION 0.5

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(mock)

//////////////////////////////////////////////////////////////////////////////
//
Player::Player() {
  _floatingTimerMax = 2;
  _floatingTimer = 0;
  _speed = PLAYER_INITIAL_SPEED;
  _maxSpeed = PLAYER_INITIAL_SPEED;
  _floating = false;
  _nextPosition = Vec2(0,0);
  _nextPosition.y = _screenSize.height * 0.6f;
  _state = kPlayerMoving;
  _jumping = false;
  _hasFloated = false;
}

PlayerMotion::PlayerMotion() {
  _floatingTimerMax = 2;
  _floatingTimer = 0;
  _speed = PLAYER_INITIAL_SPEED;
  _maxSpeed = PLAYER_INITIAL_SPEED;
  _floating = false;
  _nextPosition = Vec2(0,0);
  _nextPosition.y = _screenSize.height * 0.6f;
  _state = kPlayerMoving;
  _jumping = false;
  _hasFloated = false;
}
//////////////////////////////////////////////////////////////////////////////
//
owner<Player*> Player::create() {

  auto p= mc_new(Player);
  p->initWithFile("pics/blank.png");
  p->autorelease();
  player->setSize();
  player->initPlayer();
  return p;
}

//////////////////////////////////////////////////////////////////////////////
//
void Player::update(float dt) {

    if (_speed + P_ACCELERATION <= _maxSpeed) {
        _speed += P_ACCELERATION;
    } else {
        _speed = _maxSpeed;
    }

    _vector.x = _speed;

  switch (_state) {
    case kPlayerMoving:
      _vector.y -= FORCE_GRAVITY;
            if (_hasFloated) _hasFloated = false;
    break;

        case kPlayerFalling:

            if (_floating ) {
        _vector.y -= FLOATNG_GRAVITY;
        _vector.x *= FLOATING_FRICTION;

            } else {
        _vector.y -= FORCE_GRAVITY;
        _vector.x *= AIR_FRICTION;
        _floatingTimer = 0;
      }
    break;
        case kPlayerDying:
            _vector.y -= FORCE_GRAVITY;
            _vector.x = -_speed;
            this->setPositionX(this->getPositionX() + _vector.x);
        break;

  }

    if (_jumping) {
        _state = kPlayerFalling;
        _vector.y += PLAYER_JUMP * 0.25f;
        if (_vector.y > PLAYER_JUMP ) _jumping = false;
    }

    if (_vector.y < -TERMINAL_VELOCITY) _vector.y = -TERMINAL_VELOCITY;

    _nextPosition.y = this->getPositionY() + _vector.y;

    if (_floating) {
    _floatingTimer += dt;
    if (_floatingTimer > _floatingTimerMax) {
            _floatingTimer = 0;
      this->setFloating(false);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void PlayerMotion::reset() {

  maxSpeed.x = PLAYER_INITIAL_SPEED;
  maxSpeed.y = PLAYER_INITIAL_SPEED;
  speed.x = PLAYER_INITIAL_SPEED;
  speed.y = PLAYER_INITIAL_SPEED;
  vel = CC_ZPT;

  nextPos.y = _visRect.size.height * 0.6;
  setFloating(false);
  hasFloated = false;
  jumping = false;
}

//////////////////////////////////////////////////////////////////////////////
//
void PlayerStats::reset() {
  state = kPlayerMoving;
}

//////////////////////////////////////////////////////////////////////////////
//
void Player::reset() {
  this->setPosition(_visRect.size.width * 0.2, _visRect.size.height * 0.6);
  this->setRotation(0);
}

//////////////////////////////////////////////////////////////////////////////
//
void PlayerMotion::setFloating(bool v) {

  if (floating == v ||
      (v && hasFloated)) { return; }

  floating = v;
  if (v) {
    vel.y += HTV(PLAYER_JUMP);
    hasFloated = true;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Player::initPlayer() {

  this->setAnchorPoint(cx::anchorT());
  this->setPosition(_visRect.size.width * 0.2, _nextPosition.y);

  _height = 228;
  _width = 180;

  this->setTextureRect(c::Rect(0, 0, _width, _height));
  this->setColor(c::Color3B(255,255,255));
}


NS_END


