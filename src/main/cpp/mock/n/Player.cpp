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
owner<Player*> Player::create(const c::Rect &frame) {

  auto p= mc_new(Player);
  p->initWithFile("pics/blank.png");
  p->initPlayer(frame);
  p->autorelease();
  return p;
}

//////////////////////////////////////////////////////////////////////////////
//
PlayerMotion::PlayerMotion(const c::Rect &frame) {

  nextPos = c::Vec2(0,_visRect.size.height * 0.6);

  floatingTimerMax = 2;
  floatingTimer = 0;
  hasFloated = false;

  _floating = false;
  _jumping = false;
  _visRect=frame;

  maxSpeed.x = PLAYER_INITIAL_SPEED;
  maxSpeed.y = PLAYER_INITIAL_SPEED;
  speed.x = PLAYER_INITIAL_SPEED;
  speed.y = PLAYER_INITIAL_SPEED;
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
  _jumping = false;
  hasFloated = false;
}

//////////////////////////////////////////////////////////////////////////////
//
void PlayerMotion::setFloating(bool v) {

  if (_floating == v ||
      (v && hasFloated)) { return; }

  _floating = v;
  if (v) {
    vel.y += HTV(PLAYER_JUMP);
    hasFloated = true;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Player::initPlayer(const c::Rect &wz, const c::Vec2 &pos) {

  this->setAnchorPoint(cx::anchorT());
  //this->setPosition(wz.size.width * 0.2, _nextPosition.y);
  this->setPosition(pos.x, pos.y);

  _height = 228;
  _width = 180;
  _visRect=wz;

  this->setTextureRect(c::Rect(0, 0, _width, _height));
  this->setColor(c::Color3B(255,255,255));
}


NS_END


