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

#include "2d/CCActionInterval.h"
#include "core/ComObj.h"
#include "core/XPool.h"
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Alien : public f::ComObj {

  Alien(not_null<c::Sprite*> s, int value, int rank)
    : ComObj(s,1,value) {
    this->rank=rank;
  }

  MDECL_COMP_TPID("n/Alien")
  DECL_IZ(rank)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL AlienSquad : public a::Component {

  const s_vec<f::ComObj*>& list() { return aliens->list(); }
  AlienSquad(not_null<f::XPool*> aliens, int step) {
     this->aliens=aliens;
     this->stepx=step;
  }

  int size() { return aliens->size(); }

  MDECL_COMP_TPID("n/AlienSquad")

  //not owner of pool
  DECL_PTR(f::XPool, aliens)
  DECL_IZ(stepx)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Bomb : public f::ComObj {

  Bomb(not_null<c::Sprite*>);

  MDECL_COMP_TPID("n/Bomb")
};

//////////////////////////////////////////////////////////////////////////////
struct CC_DLL Cannon : public a::Component {

  MDECL_COMP_TPID( "n/Cannon")

  DECL_BT(hasAmmo)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Explosion : public f::ComObj {

  virtual void inflate(float x, float y);

  Explosion(not_null<c::Sprite*> s)
    : ComObj(s) {
    frameTime=0.1f ;
  }

  MDECL_COMP_TPID("n/Explosion")

  DECL_FZ(frameTime)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Looper : public a::Component {

  MDECL_COMP_TPID("n/Looper")

  virtual ~Looper();

  DECL_PTR(c::DelayTime,timer7)
  DECL_PTR(c::DelayTime,timer0)
  DECL_PTR(c::DelayTime,timer1)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Missile : public f::ComObj {

  Missile(not_null<c::Sprite*>);

  MDECL_COMP_TPID( "n/Missile")

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Motion : public a::Component {

  MDECL_COMP_TPID("n/Motion")

  DECL_BF(right)
  DECL_BF(left)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Ship : public f::ComObj {

  Ship(not_null<c::Sprite*> s,
      const sstr& s0, const sstr& s1)
    : ComObj(s) {
    frame0=s0;
    frame1=s1;
  }

  MDECL_COMP_TPID("n/Ship")

  DECL_TD(sstr, frame0)
  DECL_TD(sstr, frame1)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Slots  : public a::Component {

    MDECL_COMP_TPID("n/Slots")
  DECL_TD(c::Size, alienSize)
  DECL_TD(c::Size, shipSize)

};

NS_END(invaders)
#endif

