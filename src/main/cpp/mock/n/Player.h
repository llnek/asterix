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

#define PLAYER_INITIAL_SPEED 8
#define PLAYER_JUMP 42
#define FORCE_GRAVITY 1.5
#define FLOATNG_GRAVITY 0.4
#define TERMINAL_VELOCITY 70
#define FLOATING_FRICTION 0.98f
#define AIR_FRICTION 0.99f
#define P_ACCELERATION 0.5


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
public:
int _floatingTimerMax;
float _floatingTimer;
int _floatingInterval;
bool _hasFloated;
  void setFloating(bool value);
  CC_SYNTHESIZE(bool, _inAir, InAir);
  CC_SYNTHESIZE_READONLY(bool, _floating, Floating);
  CC_SYNTHESIZE(bool, _jumping, Jumping);
void reset();
  PlayerMotion();

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL PlayerStats : public f::CStats {
  PlayerStats() {
    state = kPlayerMoving;
  }
  void reset() {
    state = kPlayerMoving;
  }
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Player : public f::CPixie {
protected:

  virtual bool initWithFile(const sstr &fn);
  Player() {}

public:

  static owner<Player*> create();
  void reset();

};



NS_END





