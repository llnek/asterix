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

#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(astros)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL ScrollingBG : public f::CPixie {
  static owner<ScrollingBG*> create() {
    auto z= mc_new(ScrollingBG);
    z->initWithFile("pics/background.jpg");
    z->autorelease();
    return z;
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Asteroid : public f::CPixie {
  static owner<Asteroid*> create() {
    auto z= mc_new(Asteroid);
    z->initWithSpriteFrameName("asteroid.png");
    XCFG()->fit(z);
    z->autorelease();
    return z;
  }
};


//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Ship : public f::CPixie {
  static owner<Ship*> create() {
    auto z= mc_new(Ship);
    z->initWithSpriteFrameName("ship.png");
    XCFG()->fit(z);
    z->autorelease();
    return z;
  }
  __decl_bf(engineOn)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {
  __decl_comp_tpid( "n/GVars" )
  __decl_ptr(ScrollingBG, background)
  __decl_mv(int,scrollSpeed, 1)
  __decl_mv(float,gameGravity, -0.05)
  __decl_mv(float, gameThrust, 0.1)
  __decl_ptr(c::ParticleSun, emitter)

};



NS_END




