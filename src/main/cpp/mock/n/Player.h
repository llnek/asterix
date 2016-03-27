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
class CC_DLL PlayerMotion : public f::CMove {

  DECL_BF(_floating)
  DECL_BF(_jumping)
  DECL_BF(_inAir)

public:

  DECL_IZ(floatingTimerMax)
  DECL_IZ(floatingInterval)
  DECL_FZ(floatingTimer)
  DECL_BF(hasFloated)

  bool isFloating() { return _floating; }
  void setFloating(bool b);

  bool isJumping() { return _jumping; }
  void setJumping(bool b) {
    _jumping=b;
  }

  bool isInAir() { return _inAir; }
  void setInAir(bool b) {
    _inAir=b;
  }

  PlayerMotion(const c::Rect&);
}

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL PlayerStats : public f::CStats {

  PlayerStats::PlayerStats() {
    state = kPlayerMoving;
  }

  void reset() {
    state = kPlayerMoving;
  }

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Player : public f::CPixie {

  DECL_TD(c::Rect, _visRect)
  DECL_FZ(_height)
  DECL_FZ(_width)

  void initPlayer();

public:

  static owner<Player*> create();

  virtual ~Player();
  Player();

  void reset() {
    this->setPosition(_visRect.size.width * 0.2, _visRect.size.height * 0.6);
    this->setRotation(0);
  }

  virtual void place () {
    this->setPositionY( _nextPosition.y );
    if (_vector.x > 0 && this->getPositionX() < _screenSize.width * 0.2f) {
        this->setPositionX(this->getPositionX() + _vector.x);
        if (this->getPositionX() > _screenSize.width * 0.2f) {
            this->setPositionX(_screenSize.width * 0.2f);
        }
    }
  };

};


NS_END





