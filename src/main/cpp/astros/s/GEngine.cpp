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

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Resolve.h"
#include "Collide.h"
#include "Move.h"
#include "AI.h"
#include "GEngine.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(astros)

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {

  auto ent= this->reifyNode("Shared",true);
  auto ss= mc_new(GVars);
  ent->checkin(ss);

  auto po= MGMS()->reifyPool("Astros");
  po->preset([=]() -> f::Poolable* {
    auto e= this->reifyNode("Astro");
    auto sp= Asteroid::create();
    MGML()->addAtlasItem("game-pics", sp);
    CC_HIDE(sp);
    e->checkin(mc_new(f::CHealth));
    e->checkin(mc_new(f::CMove));
    e->checkin(sp);
    return e;
  }, 16);

  ent= this->reifyNode("Ship", true);
  auto ship= Ship::create();
  auto wb=cx::visBox();
  auto sz=CC_CSZ(ship);
  MGML()->addAtlasItem("game-pics",ship);
  CC_POS2(ship, wb.right * 0.1,wb.cy);
  ent->checkin(mc_new1(f::CHealth,1));
  ent->checkin(mc_new(f::CGesture));
  ent->checkin(mc_new(f::CMove));
  ent->checkin(ship);

  auto sp= cx::reifySprite("particle.png");
  XCFG()->fit(sp);
  ss->emitter = c::ParticleSun::create();
  MGML()->addItem(ss->emitter);
  ss->emitter->setTexture(sp->getTexture());
  ss->emitter->setStartSize(2);
  ss->emitter->setEndSize(4);
  CC_POS2(ss->emitter, wb.left - 100,ship->getPositionY());
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initSystems() {
  regoSystem(mc_new1(Resolve,this));
  regoSystem(mc_new1(Collide,this));
  regoSystem(mc_new1(AI,this));
  regoSystem(mc_new1(Move,this));
}


NS_END

