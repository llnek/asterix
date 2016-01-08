// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2015, Ken Leung. All rights reserved.

#if !defined(__COBJS_H__)
#define __COBJS_H__

#include "core/ComObj.h"

NS_BEGIN(asteroids)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Asteroid : public f::DynaObj {

  Asteroids(not_null<c::Sprite*> s, int value, int rank,
      float deg, float vx, float vy)
    : ComObj(s, 1, value) {
    this->rank=rank;
    this->deg= deg;
    this->vel.y= vy;
    this->vel.x= vx;
  }

  DECL_IZ(rank)

  MDECL_COMP_TPID("n/Asteroid")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Cannon : public a::Component {

  DECL_TV(float, coolDownWindow, 800.0f)
  DECL_BT(hasAmmo)

  Cannon(float w) {
    coolDownWindow= w;
  }

  MDECL_COMP_TPID("s/Cannon")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Looper : public a::Component {

  MDECL_COMP_TPID("n/Looper")

  Looper(int count) {
    //this.timers=sjs.makeArray(count,null);
  }

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Missile : public f::DynaObj {

  Missile(not_null<c::Sprite*> s, float speed=20)
    : DynaObj(s) {
    speed->vx= speed;
    speed->vy= speed;
  }

  MDECL_COMP_TPID("s/Missile")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Motion : public a::Component {

  MDECL_COMP_TPID("s/Motion")
  DECL_BF(right)
  DECL_BF(left)
  DECL_BF(up)
  DECL_BF(down)

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Ship : public f::DynaObj {

  DECL_TV(sstr,frame0,"rship_0.png")
  DECL_TV(sstr,frame1,"rship_1.png")

  Ship(not_null<c::Sprite*> s)
    : DynaObj(s) {
  }

  MDECL_COMP_TPID("s/Ship")
  DECL_TD(c::Vec2, maxVel)
};

NS_END(asteroids)
#endif


