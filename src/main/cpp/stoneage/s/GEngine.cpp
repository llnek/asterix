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
  auto p= MGMS()->reifyPools( "DiamondParticles");
  p->preset([=]() -> f::ComObj* {
    auto p = c::ParticleSystemQuad::create("pics/diamond.plist");
    p->stopSystem();
    p->setVisible(false);
    MGML()->addItem(p);
    return mc_new1(Particle,p);
  },50);

  p= MGMS()->reifyPools( "MatchParticles");
  p->preset([=]() -> f::ComObj* {
    auto p = c::ParticleSystemQuad::create("pics/match.plist");
    p->stopSystem();
    p->setVisible(false);
    MGML()->addItem(p);
    return mc_new1(Particle,p);
  },50);

  p= MGMS()->reifyPools( "Diamonds");
  p->preset([=]() -> f::ComObj* {
    auto p = cx::loadSprite("gem_white.png");
    p->setLocalZOrder(Z_DIAMOND);
    p->setVisible(false);
    MGML()->addItem(p);
    return mc_new1(Diamond,p);
  },50);

}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initSystems() {

}


NS_END

