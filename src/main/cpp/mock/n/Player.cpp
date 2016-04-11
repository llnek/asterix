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
NS_BEGIN(mock)

//////////////////////////////////////////////////////////////////////////////
//
owner<Player*> Player::create() {

  auto p= mc_new(Player);
  p->initWithFile("pics/blank.png");
  p->autorelease();
  return p;
}

//////////////////////////////////////////////////////////////////////////////
//
bool Player::initWithFile(const sstr &fn) {
  auto rc= c::Sprite::initWithFile(fn);
  if (!rc) { return false; }

  this->setTextureRect(c::Rect(0, 0, 180, 228));
  this->setAnchorPoint(cx::anchorT());
  this->setColor(c::Color3B(255,255,255));
  reset();

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Player::reset() {
  auto wb= cx::visBox();
  this->setPosition(wb.right * 0.2, wb.top * 0.6);
  this->setRotation(0);
}

//////////////////////////////////////////////////////////////////////////////
//
PlayerMotion::PlayerMotion() {

  auto wb= cx::visBox();

  _floatingTimerMax = 2;
  _floatingTimer = 0;

  nextPos= CCT_PT(0, wb.top * 0.6);
  maxSpeed.x = PLAYER_INITIAL_SPEED;
  speed.x = PLAYER_INITIAL_SPEED;

  _floating = false;
  _jumping = false;
  _hasFloated = false;
}

//////////////////////////////////////////////////////////////////////////////
//
void PlayerMotion::reset() {

  auto wb= cx::visBox();

  maxSpeed.x = PLAYER_INITIAL_SPEED;
  speed.x = PLAYER_INITIAL_SPEED;

  vel = CC_ZPT;

  this->setFloating(false);

  nextPos.y = wb.top * 0.6;
  _jumping = false;
  _hasFloated = false;
}

//////////////////////////////////////////////////////////////////////////////
//
void PlayerMotion::setFloating(bool value) {
  if (_floating == value ||
      (value && _hasFloated)) { return; }
  _floating = value;
  if (value) {
    vel.y += HTV(PLAYER_JUMP);
    _hasFloated = true;
  }
}



NS_END


