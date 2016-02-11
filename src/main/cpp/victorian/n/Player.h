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

#include "C.h"

#define PLAYER_INITIAL_SPEED 8
#define PLAYER_JUMP 42
#define G_FORCE 1.5
#define FLOATNG_GRAVITY 0.4
#define TERMINAL_VELOCITY 70
#define FLOATING_FRICTION 0.98f
#define AIR_FRICTION 0.99f

NS_BEGIN(victorian)

typedef enum {
    kPlayerMoving,
    kPlayerFalling,
    kPlayerDying

} PlayerState;

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Player : public Widget {

  Action *_rideAnimation;
  Action *_floatAnimation;

  bool _hasFloated;

  int _floatingTimerMax;
  float _floatingTimer;
  int _floatingInterval;

  void initPlayer();

  Player(not_null<c::Sprite*>);
  virtual ~Player();

  static Player* create();

  virtual void update (float dt);
  void setFloating (bool value);
  void reset();

  virtual void place () {
    auto wb=cx::visBox();
    sprite->setPositionY(nextPos.y);
    if (vel.x > 0 && sprite->getPositionX() < wb.right * 0.2f) {
      sprite->setPositionX(sprite->getPositionX() + vel.x);
      if (sprite->getPositionX() > wb.right * 0.2f) {
        sprite->setPositionX(wb.right * 0.2f);
      }
    }
  }

  virtual float left() {
    return sprite->getPositionX() - _width * 0.5f;
  }

  virtual float right() {
    return sprite->getPositionX() + _width * 0.5f;
  }

  virtual float top() {
    return sprite->getPositionY() ;
  }

  virtual float bottom() {
    return sprite->getPositionY() - _height  ;
  }

  virtual float next_left() {
    return nextPos.x - _width * 0.5f;
  }

  virtual float next_right() {
    return nextPos.x + _width * 0.5f;
  }

  virtual float next_top() {
    return nextPos.y ;
  }

  virtual float next_bottom() {
    return nextPos.y - _height;
  }

  CC_SYNTHESIZE(PlayerState, _state, State);
  CC_SYNTHESIZE(bool, _inAir, InAir);
  CC_SYNTHESIZE_READONLY(bool, _floating, Floating);
  CC_SYNTHESIZE(bool, _jumping, Jumping);
};





NS_END




