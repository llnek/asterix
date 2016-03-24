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

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(astros)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL ScrollingBG : public f::CPixie {
  static owner<ScrollingBG*> create() {
    auto z= mc_new(ScrollingBG);
    z->initWithFile("pics/background.png");
    z->autorelease();
    return z;
  }
  virtual ~ScrollingBG() {}
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Asteroid : public f::CPixie {
  static owner<Asteroid*> create() {
    auto z= mc_new(Asteroid);
    z->initWithSpriteFrameName("asteroid.png");
    z->autorelease();
    return z;
  }
  virtual ~Asteroid() {}
};


//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Ship : public f::CPixie {
  static owner<Ship*> create() {
    auto z= mc_new(Ship);
    z->initWithSpriteFrameName("ship.png");
    z->autorelease();
    return z;
  }
  virtual ~Ship() {}
  DECL_BF(engineOn)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {
  MDECL_COMP_TPID( "n/GVars" )
  DECL_PTR(ScrollingBG, background)
  DECL_TV(int,scrollSpeed, 1)
  DECL_TV(float,gameGravity, -0.05)
  DECL_TV(float, gameThrust, 0.1)
  DECL_PTR(c::ParticleSun, emitter)

};



NS_END




