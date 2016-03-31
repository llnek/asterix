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

  __decl_comp_tpid("n/Asteroid")
  __decl_iz(rank)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Cannon : public ecs::Component {

  __decl_comp_tpid("n/Cannon")
  __decl_bt(hasAmmo)

};

//////////////////////////////////////////////////////////////////////////////
//missile speed=20

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Ship : public f::CPixie {

  __decl_mv(sstr,frame0,"rship_0.png")
  __decl_mv(sstr,frame1,"rship_1.png")
  //__decl_comp_tpid("n/Ship")
  __decl_comp_tpid("f/CPixie")

  Ship(not_null<c::Node*> s)
    : CPixie(s) {
  }

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {
  __decl_comp_tpid("n/GVars")
  __decl_md(c::Size, playerSize)
  __decl_md(c::Size, ufoSize)
  __decl_md(c::Size, astro3)
  __decl_md(c::Size, astro2)
  __decl_md(c::Size, astro1)

};


NS_END


