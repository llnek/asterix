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

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(victorian)

//////////////////////////////////////////////////////////////////////////////
//
bool Player::initWithSpriteFrameName(const sstr &fn) {
  auto rc= c::Sprite::initWithSpriteFrameName(fn);
  auto wb=cx::visBox();
  if (!rc) { return false; }

  this->setPosition(wb.right * 0.2, wb.top * 0.6);
  this->setAnchorPoint(cx::anchorT());

  auto anim= c::Animation::create();
  anim->setRestoreOriginalFrame(false);
  anim->setDelayPerUnit(0.2 / 3.0);
  anim->setLoops(-1);
  for (auto i = 1; i <= 3; ++i) {
    anim->addSpriteFrame(
      cx::getSpriteFrame("player_"+FTOS(i)+".png"));
  }
  _rideAnimation = c::Animate::create(anim);
  CC_KEEP(_rideAnimation);

  auto easeSwing = c::Sequence::create(
         c::EaseInOut::create(c::RotateTo::create(0.8, -10), 2),
         c::EaseInOut::create(c::RotateTo::create(0.8, 10), 2),
         CC_NIL);
  _floatAnimation = c::RepeatForever::create( (c::ActionInterval*) easeSwing );
  CC_KEEP(_floatAnimation);

  animateRide();
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
owner<Player*> Player::create() {
  auto p= mc_new(Player);
  p->initWithSpriteFrameName("player_1.png");
  p->autorelease();
  return p;
}

//////////////////////////////////////////////////////////////////////////////
//
void Player::animateFloat() {
  this->runAction(_floatAnimation);
}

//////////////////////////////////////////////////////////////////////////////
//
void Player::animateRide() {
  this->runAction(_rideAnimation);
}

//////////////////////////////////////////////////////////////////////////////
//
Player::~Player(){
  CC_DROP(_floatAnimation);
  CC_DROP(_rideAnimation);
}

//////////////////////////////////////////////////////////////////////////////
//
PlayerMotion::PlayerMotion() {
  auto wb=cx::visBox();
  maxSpeed.x = PLAYER_INITIAL_SPEED;
  speed.x = PLAYER_INITIAL_SPEED;
  nextPos= CCT_PT(0, wb.top * 0.6);
}

//////////////////////////////////////////////////////////////////////////////
//
void PlayerMotion::setFloating(Player *py, bool value) {

  if (_floating == value ||
      (value && _hasFloated)) {
  return; }

  py->stopAllActions();
  _floating = value;

  if (value) {
    py->setDisplayFrame(cx::getSpriteFrame("player_float.png"));
    _hasFloated = true;
    py->animateFloat();
    vel.y += HTV(PLAYER_JUMP);
    cx::sfxPlay("openUmbrella");
  } else {
    py->animateRide();
  }
}



NS_END


