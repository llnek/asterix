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
#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"
#include "lib.h"
#include "Player.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(blast)

//////////////////////////////////////////////////////////////////////////////
//
bool Player::init() {

  if (! c::DrawNode::init()) {
  return false; }

  // generate vertices for the player
  c::Vec2 vs[] = {
    c::Vec2(PLAYER_RADIUS * 1.75, 0),
    c::Vec2(PLAYER_RADIUS * -0.875, PLAYER_RADIUS),
    c::Vec2(PLAYER_RADIUS * -1.75, 0),
    c::Vec2(PLAYER_RADIUS * -0.875, PLAYER_RADIUS * -1)
  };

  // draw a green coloured player
  drawPolygon(vs, 4, c::ccc4f(0,0,0,0), 1.5, c::ccc4f(0,1,0,1));
  scheduleUpdate();
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Player::update(float dt) {
  c::DrawNode::update(dt);
  auto prev= getPosition();
  updatePosition();
  updateRotation(prev);
}

//////////////////////////////////////////////////////////////////////////////
//
void Player::updatePosition() {
  auto box= MGMS()->getEnclosureRect();
    auto pt= getPosition();
  // don't move if speed is too low
  if (c::ccpLength(speed) > 0.75) {
    // add speed but limit movement within the boundary
    auto nextpos= c::ccpAdd(pt, speed);
    if (RECT_CONTAINS_CIRCLE(box, nextpos, PLAYER_RADIUS)) {
      setPosition(nextpos);
    }
    else
    if (RECT_CONTAINS_CIRCLE(box, c::Vec2(nextpos.x - speed.x, nextpos.y), PLAYER_RADIUS)) {
      setPosition(nextpos.x - speed.x, nextpos.y);
    }
    else
    if (RECT_CONTAINS_CIRCLE(box, c::Vec2(nextpos.x, nextpos.y - speed.y), PLAYER_RADIUS)) {
      setPosition(nextpos.x, nextpos.y - speed.y);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Player::updateRotation(const c::Vec2 &prev) {
    auto pt= getPosition();
  // don't rotate if speed is too low
  if (c::ccpLength(speed) > 1) {
    auto prevrot= getRotation();
    // calculate target angle based on previous & current position
    auto targetrot= CC_RADIANS_TO_DEGREES(c::ccpToAngle(c::ccpSub(pt, prev)) * -1);

    // add some easing to the rotation
    auto rotstep = 0.0f;
    if (targetrot > 90 && prevrot < -90) {
      rotstep = (360.0 - fabs(targetrot - prevrot)) / -4;
    }
    else if(targetrot < -90 && prevrot > 90) {
      rotstep = (360.0 - fabs(targetrot - prevrot)) / 4;
    }
    else {
      rotstep = (targetrot - prevrot) / 4;
    }

    // limit rotation between -180 & 180 degrees
    auto finalrot = prevrot + rotstep;
    if ( finalrot > 180) {
      finalrot -= 360;
    }
    else if( finalrot < -180) {
      finalrot += 360;
    }

    setRotation(finalrot);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Player::die() {
  // don't die if already dying
  if (isDying) {
  return; }

  // stop moving
  isDying = true;
  speed.x = 0;
  speed.y = 0;

  auto shake_duration = 0.5;
  auto num_shakes = 8;
  // create & animate the death and end the game afterwards
  auto shake = c::Spawn::createWithTwoActions(
      c::ScaleTo::create(shake_duration, 1.2),
      c::Repeat::create(
        c::Sequence::create(
          c::RotateBy::create(shake_duration/(num_shakes*2), -20),
          c::RotateBy::create(shake_duration/(num_shakes*2), 20),
          CC_NIL), num_shakes));
  auto shrink = c::EaseSineIn::create(c::ScaleTo::create(0.1, 0.0));

  runAction(
      c::Sequence::create(
        c::Sequence::create(shake, shrink, CC_NIL),
        c::CallFunc::create([=]() { this->dead(); }),
        CC_NIL));

  cx::sfxPlay("blast_player");
}

//////////////////////////////////////////////////////////////////////////////
//
void Player::dead() {
  SENDMSG("/game/player/lose");
}

//////////////////////////////////////////////////////////////////////////////
//
void Player::setShield(Shield *s) {
  _shield = s;
  // with shield enabled, the collision radius must be increased to the size of the shield
  radius = _shield ? PLAYER_RADIUS * 3 : PLAYER_RADIUS;
}


NS_END


