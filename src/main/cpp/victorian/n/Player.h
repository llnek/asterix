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

#include "core/COMP.h"

NS_BEGIN(victorian)

enum PlayerState {
  kPlayerMoving,
  kPlayerFalling,
  kPlayerDying
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Player : public f::CPixie {

  virtual bool initSpriteWithFrameName(const sstr&);
  Player();

  __decl_ptr(c::Action, _floatAnimation)
  __decl_ptr(c::Action, _rideAnimation)

public:

  static owner<Player*> create();
  virtual ~Player();

  void animateFloat();
  void animateRide();

  inline virtual void place () {
    this->setPositionY( _nextPosition.y );
        if (_vector.x > 0 && this->getPositionX() < _screenSize.width * 0.2f) {
            this->setPositionX(this->getPositionX() + _vector.x);
            if (this->getPositionX() > _screenSize.width * 0.2f) {
                this->setPositionX(_screenSize.width * 0.2f);
            }
        }
  };
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL PlayerMotion : public f::CMove {

  __decl_iz(_floatingTimerMax)
  __decl_fz(_floatingTimer)
  __decl_iz(_floatingInterval)
  __decl_bf(_hasFloated)
  __decl_bf(_floating)
  __decl_bf(_jumping)
  __decl_bf(_inAir)

  PlayerMotion();

public:

  __declapi_setr(bool, _floating, Floating)
  __decl_gsms_is(_jumping, Jumping)
  __decl_gsms_is(_inAir, InAir)
  __decl_ismx(_floating, Floating)

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL PlayerStats : public f::CStats {
  void reset() { state = kPlayerMoving; }
  PlayerStats() {
    reset();
  }
};



NS_END





