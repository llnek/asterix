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

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Resolve.h"
#include "Collide.h"
#include "Move.h"
#include "AI.h"
#include "GEngine.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(stoneage)

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {
  auto po= MGMS()->reifyPool("DiamondParticles");
  po->preset([=]() -> f::Poolable* {
    auto p = c::ParticleSystemQuad::create(XCFG()->getAtlas("diamonds"));
    p->stopSystem();
    CC_HIDE(p);
    MGML()->addItem(p);
    auto ent= this->reifyNode("DiamondParticle");
    ent->checkin(mc_new1(Particle,p));
    return ent;
  }, 32);

  po= MGMS()->reifyPool( "MatchParticles");
  po->preset([=]() -> f::Poolable* {
    auto p = c::ParticleSystemQuad::create(
        XCFG()->getAtlas("matches"));
    p->stopSystem();
    CC_HIDE(p);
    MGML()->addItem(p);
    auto ent= this->reifyNode("MatchParticle");
    ent->checkin(mc_new1(Particle,p));
    return ent;
  },32);

  po= MGMS()->reifyPool( "Diamonds");
  po->preset([=]() -> f::Poolable* {
    auto p = Diamond::create();
    CC_HIDE(p);
    MGML()->addItem(p);
    auto ent= this->reifyNode("Diamond");
    ent->checkin(p);
    return ent;
  },32);

  auto ent= this->reifyNode("Arena", true);
  auto ss= mc_new(GVars);
  ent->checkin(ss);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initSystems() {

}


NS_END

