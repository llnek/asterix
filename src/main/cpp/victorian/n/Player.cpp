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

#define P_ACCELERATION 0.05

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(victorian)

//////////////////////////////////////////////////////////////////////////////
//
Player::~Player(){
  CC_SAFE_RELEASE(_floatAnimation);
  CC_SAFE_RELEASE(_rideAnimation);
}

//////////////////////////////////////////////////////////////////////////////
//
Player::Player(not_null<c::Node*> s)
  : Widget(s) {

  auto wb=cx::visBox();

  maxSpeed = PLAYER_INITIAL_SPEED;
  speed = PLAYER_INITIAL_SPEED;

  _floatingTimerMax = 2;
  _floatingTimer = 0;
  _floating = false;
  nextPos= c::Vec2(0,0);
  nextPos.y = wb.top * 0.6;
  _state = kPlayerMoving;
  _jumping = false;
  _hasFloated = false;
}

//////////////////////////////////////////////////////////////////////////////
//
Player* Player::create() {

  auto s= cx::reifySprite("player_1.png");
  auto player = mc_new1(Player,s);

  player->setSize();
  player->initPlayer();
  return player;
}

//////////////////////////////////////////////////////////////////////////////
//
void Player::update (float dt) {

  if (speed + P_ACCELERATION <= maxSpeed) {
    speed += P_ACCELERATION;
  } else {
    speed = maxSpeed;
  }

  vel.x = speed;

  switch (_state) {
    case kPlayerMoving:
      vel.y -= G_FORCE;
      if (_hasFloated) _hasFloated = false;
    break;

    case kPlayerFalling:
      if (_floating ) {
        vel.y -= FLOATNG_GRAVITY;
        vel.x *= FLOATING_FRICTION;
      } else {
        vel.y -= G_FORCE;
        vel.x *= AIR_FRICTION;
        _floatingTimer = 0;
      }
    break;

    case kPlayerDying:
      vel.y -= G_FORCE;
      vel.x = -speed;
      node->setPositionX(node->getPositionX() + vel.x);
    break;
  }

  if (_jumping) {
    _state = kPlayerFalling;
    vel.y += PLAYER_JUMP * 0.25;
    if (vel.y > PLAYER_JUMP ) _jumping = false;
  }

  if (vel.y < -TERMINAL_VELOCITY) vel.y = -TERMINAL_VELOCITY;

  nextPos.y = node->getPositionY() + vel.y;

  if (vel.x * vel.x < 0.01) vel.x = 0;
  if (vel.y * vel.y < 0.01) vel.y = 0;

  if (_floating) {
    _floatingTimer += dt;
    if (_floatingTimer > _floatingTimerMax) {
      cx::sfxPlay("falling");
      _floatingTimer = 0;
      this->setFloating(false);
    }
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void Player::reset () {

  auto wb=cx::visBox();

  maxSpeed = PLAYER_INITIAL_SPEED;
  speed = PLAYER_INITIAL_SPEED;

  vel = c::Vec2(0,0);

  this->setFloating(false);
  node->setRotation(0);

  nextPos.y = wb.top * 0.6;
  node->setPosition(wb.right * 0.2, nextPos.y);
  _state = kPlayerMoving;
  _jumping = false;
  _hasFloated = false;
}

//////////////////////////////////////////////////////////////////////////////
//
void Player::setFloating (bool value) {

  if (_floating == value) {
  return; }
  if (value && _hasFloated) {
  return; }

  _floating = value;
  node->stopAllActions();

  if (value) {
    _hasFloated = true;
    cx::sfxPlay("openUmbrella");
      SCAST(c::Sprite*,node)->setDisplayFrame(cx::getSpriteFrame("player_float.png"));
    node->runAction(_floatAnimation);
    vel.y += HTV(PLAYER_JUMP);
  } else {
    node->runAction(_rideAnimation);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Player::initPlayer() {

  auto wb=cx::visBox();

  node->setAnchorPoint(cx::anchorT());
  node->setPosition(wb.right * 0.2, nextPos.y);

  _height = 252 * 0.95;
  _width = 184;

  auto anim= c::Animation::create();
  for (auto i = 1; i <= 3; ++i) {
    auto f= cx::getSpriteFrame("player_"+s::to_string(i)+".png");
    anim->addSpriteFrame(f);
  }
  anim->setRestoreOriginalFrame(false);
  anim->setDelayPerUnit(0.2 / 3.0);
  anim->setLoops(-1);
  _rideAnimation = c::Animate::create(anim);
  CC_KEEP(_rideAnimation);

  auto easeSwing = c::Sequence::create(
         c::EaseInOut::create(c::RotateTo::create(0.8, -10), 2),
         c::EaseInOut::create(c::RotateTo::create(0.8, 10), 2),
         CC_NIL);
  _floatAnimation = c::RepeatForever::create( (c::ActionInterval*) easeSwing );
  CC_KEEP(_floatAnimation);

  node->runAction(_rideAnimation);
}



NS_END


