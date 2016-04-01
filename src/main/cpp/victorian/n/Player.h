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

NS_BEGIN(victorian)

enum PlayerState {
  kPlayerMoving,
  kPlayerFalling,
  kPlayerDying
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Player : public f::CPixie {
protected:

  bool initWithSpriteFrameName(const sstr&);

  __decl_ptr(c::Action, _floatAnimation)
  __decl_ptr(c::Action, _rideAnimation)

  Player() {}

public:

  static owner<Player*> create();
  void animateFloat();
  void animateRide();
  virtual ~Player();
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL PlayerStats : public f::CStats {
  void reset() { state = kPlayerMoving; }
  PlayerStats() {
    reset();
  }
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL PlayerMotion : public f::CMove {

  __decl_mv(float,_floatingTimerMax, 2)
  __decl_fz(_floatingTimer)
  __decl_bf(_floating)
  __decl_bf(_jumping)
  __decl_bf(_hasFloated)
  __decl_bf(_inAir)

public:

  bool isFloating() { return _floating; }
  void setFloating(Player*, bool);

  __decl_gsms(float,_floatingTimerMax,FloatingTimerMax)
  __decl_gsms(float,_floatingTimer,FloatingTimer)
  __decl_gsms(bool,_hasFloated,HasFloated)

  __decl_gsms_is(_jumping, Jumping)
  __decl_gsms_is(_inAir, InAir)

  PlayerMotion();
};




NS_END




