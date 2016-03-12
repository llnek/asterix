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
NS_BEGIN(asteroids)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Asteroid : public f::CStats {

  Asteroid(int value, int rank)
    : CStats(value) {
    this->rank= rank;
  }

  MDECL_COMP_TPID("n/Asteroid")
  DECL_IZ(rank)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Cannon : public ecs::Component {

  MDECL_COMP_TPID("n/Cannon")
  DECL_BT(hasAmmo)

};

//////////////////////////////////////////////////////////////////////////////
//missile speed=20

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Ship : public f::CDraw {

  DECL_TV(sstr,frame0,"rship_0.png")
  DECL_TV(sstr,frame1,"rship_1.png")
  MDECL_COMP_TPID("n/Ship")

  Ship(not_null<c::Node*> s)
    : CDraw(s) {
  }

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {
  MDECL_COMP_TPID("n/GVars")
  DECL_TD(c::Size, playerSize)
  DECL_TD(c::Size, ufoSize)
  DECL_TD(c::Size, astro3)
  DECL_TD(c::Size, astro2)
  DECL_TD(c::Size, astro1)

};


NS_END


