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
#include "Rocket.h"
#include "LineContainer.h"

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
struct CC_DLL LineDrawings : public ecs::Component {
  LineDrawings(not_null<LineContainer*> c) {
    lines=c;
  }
  DECL_PTR(LineContainer,lines)
  MDECL_COMP_TPID("n/LineDrawings")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Planet : public f::CPixie {
  Planet(not_null<c::Node*> s)
  : CPixie(s) {}
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {

  DECL_PTR(c::ParticleSystemQuad, pickup)
  DECL_PTR(c::ParticleSystemQuad, jet)
  DECL_PTR(c::ParticleSystemQuad, boom)
  DECL_PTR(c::ParticleSystemQuad, comet)
  DECL_PTR(c::ParticleSystemQuad, warp)
  DECL_PTR(c::ParticleSystemQuad, star)

  MDECL_COMP_TPID( "n/GVars" )
  DECL_FZ(minLineLength)
  DECL_BF(drawing)
  DECL_IZ(gridPos)
  DECL_IZ(state)
  DECL_IZ(cometInterval)
  DECL_IZ(cometTimer)
  DECL_FZ(timeBetweenPickups)

  s_vec<c::Vec2> grid;
};



NS_END




