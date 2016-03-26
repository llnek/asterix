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

#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"
#include "GameSprite.h"

#define PLAYER_INITIAL_SPEED 8
#define PLAYER_JUMP 42
#define FORCE_GRAVITY 1.5
#define FLOATNG_GRAVITY 0.4
#define TERMINAL_VELOCITY 70
#define FLOATING_FRICTION 0.98f
#define AIR_FRICTION 0.99f

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(mock)

enum PlayerState {
  kPlayerMoving,
  kPlayerFalling,
  kPlayerDying
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Player : public GameSprite {

    float _speed;
  int _floatingTimerMax;
  float _floatingTimer;
  int _floatingInterval;
  bool _hasFloated;

  Size _screenSize;

  void initPlayer();

public:

  Player(void);
  virtual ~Player(void);

  CC_SYNTHESIZE(PlayerState, _state, State);
  CC_SYNTHESIZE(bool, _inAir, InAir);
  CC_SYNTHESIZE_READONLY(bool, _floating, Floating);
    CC_SYNTHESIZE(bool, _jumping, Jumping);
    CC_SYNTHESIZE(float, _maxSpeed, MaxSpeed);

    static Player * create (void);

  virtual void update (float dt);

    void setFloating (bool value);

    void reset (void);

  inline virtual void place () {
    this->setPositionY( _nextPosition.y );
        if (_vector.x > 0 && this->getPositionX() < _screenSize.width * 0.2f) {
            this->setPositionX(this->getPositionX() + _vector.x);
            if (this->getPositionX() > _screenSize.width * 0.2f) {
                this->setPositionX(_screenSize.width * 0.2f);
            }
        }
  };

    inline int left() {
      return this->getPositionX() - _width * 0.5f;
  }

  inline int right() {
      return this->getPositionX() + _width * 0.5f;
  }

    inline int top() {
      return this->getPositionY() ;
    }

    inline int bottom() {
    return this->getPositionY() - _height ;
    }

    inline int next_left() {
      return _nextPosition.x - _width * 0.5f;
    }

    inline int next_right() {
      return _nextPosition.x + _width * 0.5f;
    }

    inline int next_top() {
      return _nextPosition.y ;
    }

    inline int next_bottom() {
      return _nextPosition.y - _height;
    }

};


NS_END





