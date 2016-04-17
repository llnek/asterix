// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "core/COMP.h"
NS_BEGIN(asteroids)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Asteroid : public f::CPixie {

    Asteroid(int rank) {
    this->_rank= rank;
  }

  __decl_iz(_rank)

public:

  __decl_getr(int, _rank,Rank)

  static owner<Asteroid*> create(const sstr &img, int rank) {
    auto z= mc_new1(Asteroid, rank);
    z->initWithSpriteFrameName(img);
    XCFG()->fit(z);
    z->autorelease();
    return z;
  }

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL ShipStats : public f::CStats {
  __decl_bt(hasAmmo)
};

//////////////////////////////////////////////////////////////////////////////
//missile speed=20

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Ship : public f::CPixie {

  __decl_mv(sstr,frame0,"rship_0.png")
  __decl_mv(sstr,frame1,"rship_1.png")

  static owner<Ship*> create() {
    auto z= mc_new(Ship);
    z->initWithSpriteFrameName("rship_0.png");
    XCFG()->fit(z);
    z->autorelease();
    return z;
  }

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {
  __decl_comp_tpid("n/GVars")
  __decl_md(CCT_SZ, playerSize)
  __decl_md(CCT_SZ, ufoSize)
  __decl_md(CCT_SZ, astro3)
  __decl_md(CCT_SZ, astro2)
  __decl_md(CCT_SZ, astro1)

};


NS_END


