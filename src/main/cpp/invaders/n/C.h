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

#include "2d/CCActionInterval.h"
#include "core/COMP.h"
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Alien : public f::CDraw {

  Alien(not_null<c::Node*> s)
    : CDraw(s) {
  }

  MDECL_COMP_TPID("n/Alien")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL AlienSquad : public ecs::Component {

  const s_vec<f::Poolable*>& list() { return aliens->list(); }

  AlienSquad(not_null<f::FPool*> aliens, int step) {
    this->aliens=aliens;
    this->stepx=step;
  }

  int size() { return aliens->size(); }

  MDECL_COMP_TPID("n/AlienSquad")

  //not owner of pool
  DECL_PTR(f::FPool, aliens)
  DECL_IZ(stepx)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Bomb : public f::CDraw {

  Bomb(not_null<c::Node*> s)
    : CDraw(s) {
  }
  MDECL_COMP_TPID("n/Bomb")
};

//////////////////////////////////////////////////////////////////////////////
struct CC_DLL Cannon : public ecs::Component {

  MDECL_COMP_TPID( "n/Cannon")
  DECL_BT(hasAmmo)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Explosion : public f::CDraw {

  virtual void inflate(float x, float y);

  Explosion(not_null<c::Node*> s)
    : CDraw(s) {
    frameTime=0.1f ;
  }

  MDECL_COMP_TPID("n/Explosion")
  DECL_FZ(frameTime)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Looper : public ecs::Component {

  MDECL_COMP_TPID("n/Looper")
  virtual ~Looper();

  DECL_PTR(c::DelayTime, timer7)
  DECL_PTR(c::DelayTime, timer0)
  DECL_PTR(c::DelayTime, timer1)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Missile : public f::CDraw {

  MDECL_COMP_TPID( "n/Missile")
  Missile(not_null<c::Node*> s)
  : CDraw(s) {
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Ship : public f::CDraw {

  Ship(not_null<c::Node*> s,
      const sstr& s0, const sstr& s1)
    : CDraw(s) {
    frame0=s0;
    frame1=s1;
  }

  MDECL_COMP_TPID("n/Ship")
  DECL_TD(sstr, frame0)
  DECL_TD(sstr, frame1)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars  : public ecs::Component {

  DECL_TD(c::Size, alienSize)
  DECL_TD(c::Size, shipSize)
  MDECL_COMP_TPID("n/GVars")

};



NS_END


