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

enum PlayerState {
  kPlayerMoving,
  kPlayerFalling,
  kPlayerDying
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Player : public Widget {

  DECL_PTR(c::Action, _floatAnimation)
  DECL_PTR(c::Action, _rideAnimation)

  DECL_BF(_hasFloated)

  DECL_IZ(_floatingInterval)
  DECL_IZ(_floatingTimerMax)
  DECL_FZ(_floatingTimer)

  void initPlayer();

  Player(not_null<c::Node*>);
  virtual ~Player();

  static Player* create();

  virtual void update (float dt);
  void setFloating (bool value);
  void reset();

  virtual void place () {
    auto wb=cx::visBox();
    node->setPositionY(nextPos.y);
    if (vel.x > 0 &&
        node->getPositionX() < wb.right * 0.2) {
      node->setPositionX(node->getPositionX() + vel.x);
      if (node->getPositionX() > wb.right * 0.2) {
        node->setPositionX(wb.right * 0.2);
      }
    }
  }

  virtual float left() {
    return node->getPositionX() - HTV(_width);
  }

  virtual float right() {
    return node->getPositionX() + HTV(_width);
  }

  virtual float top() {
    return node->getPositionY() ;
  }

  virtual float bottom() {
    return node->getPositionY() - _height  ;
  }

  virtual float next_left() {
    return nextPos.x - HTV(_width);
  }

  virtual float next_right() {
    return nextPos.x + HTV(_width);
  }

  virtual float next_top() {
    return nextPos.y ;
  }

  virtual float next_bottom() {
    return nextPos.y - _height;
  }

  //MDECL_COMP_TPID("n/Player")
  MDECL_COMP_TPID("f/CPixie")

  CC_SYNTHESIZE_READONLY(bool, _floating, Floating);
  CC_SYNTHESIZE(PlayerState, _state, State);
  CC_SYNTHESIZE(bool, _inAir, InAir);
  CC_SYNTHESIZE(bool, _jumping, Jumping);
};





NS_END




