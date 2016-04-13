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
#include "Rocket.h"
#include "RPath.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(rocket)

enum {
  kBackground,
  kMiddleground,
  kForeground
};

enum {
  kSpriteRocket,
  kSpritePlanet,
  kSpriteBoost,
  kSpriteStar
};

enum GameState {
  kGameIntro,
  kGamePaused,
  kGamePlay,
  kGameOver
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Planet : public f::CPixie {
  Planet() {}
public:
  static owner<Planet*> create(int t) {
    auto s= mc_new(Planet);
    s->initWithSpriteFrameName("planet_"+FTOS(t)+".png");
    s->autorelease();
    return s;
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {

  __decl_ptr(c::ParticleSystemQuad, pickup)
  __decl_ptr(c::ParticleSystemQuad, jet)
  __decl_ptr(c::ParticleSystemQuad, boom)
  __decl_ptr(c::ParticleSystemQuad, comet)
  __decl_ptr(c::ParticleSystemQuad, warp)
  __decl_ptr(c::ParticleSystemQuad, star)

  __decl_comp_tpid( "n/GVars" )
  __decl_vec(CCT_PT, grid)

  __decl_fz(timeBetweenPickups)
  __decl_fz(minLineLength)
  __decl_bf(drawing)
  __decl_iz(gridPos)
  __decl_iz(state)
  __decl_iz(cometInterval)
  __decl_iz(cometTimer)

};



NS_END




