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

#include "core/CCSX.h"
#include "lib.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(monsters)

//////////////////////////////////////////////////////////////////////////////
//
s_vec<ecs::Entity*> getEntsOnTeam(GEngine *engine, int team,  const COMType &ct) {

  auto ents= engine->getEntities(ct, ents);
  s_vec<ecs::Entity*> out;

  F__LOOP(it, ents) {
    auto e = *it;
    auto cur = (Team*) e->get("n/Team");
    if (cur && cur->team == team) {
      out.push_back(e);
    }
  }

  return out;
}

//////////////////////////////////////////////////////////////////////////////
//
Entity* closestEntOnTeam(GEngine *engine, Entity *ent, int team) {

  auto ourRender = CC_GEC(f::CDraw,ent,"f/CDraw");
  if (!ourRender) { return nullptr; }

  auto others = getEntsOnTeam(engine, team, "f/CDraw");
  Entity *closestEnt= nullptr;
  float closestDist= -1;

  F__LOOP(it, others) {
    auto e= *it;
    auto r2= CC_GEC(f::CDraw,e,"f/CDraw");
    auto distance = c::ccpDistance(ourRender->pos(), r2->pos());
    if (distance < closestDist || closestDist == -1) {
      closestEnt= e;
      closestDist = distance;
    }
  }

  return closestEnt;
}


NS_END




