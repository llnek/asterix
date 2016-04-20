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
NS_BEGIN(cuteness)

//////////////////////////////////////////////////////////////////////////////
//
static s_arr<sstr,3> ENEMYPICS = {
  "beetleship.png",
  "octopus.png",
  "rocketship.png"
};

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {
  // global
  auto ent= this->reifyNode("Shared",true);
  auto ss= mc_new(GVars);
  auto wb=cx::visBox();

  ent->checkin(ss);

  //planet
  ent= this->reifyNode("Planet", true);
  auto p= Planet::create();
  CC_POS2(p, wb.cx, wb.cy);
  MGML()->addAtlasItem("gpics",p);
  ent->checkin(p);
  ent->checkin(mc_new(f::CHealth));
  ent->checkin(mc_new(f::CHuman));

  auto sq= mc_new(EnemySquad);
  F__LOOP(it,ENEMYPICS) {
    auto &n= *it;
    auto p= MGMS()->reifyPool(n);
    p->preset([=]() -> f::Poolable* {
      auto e= this->reifyNode(n);
      auto z= Enemy::create(n);
      MGML()->addAtlasItem("gpics", z);
      CC_HIDE(z);
      e->checkin(z);
      e->checkin(mc_new(f::CHealth));
      return e;
    }, 16);
    sq->add(p);
  }
  ent->checkin(sq);
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

