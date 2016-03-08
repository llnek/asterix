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
#include "GEngine.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(monsters)

//////////////////////////////////////////////////////////////////////////////
//
s_vec<ecs::Entity*> getEntsOnTeam(
    ecs::Engine *engine, int team,  const ecs::COMType &ct) {

  auto ents= SCAST(GEngine*,engine)->getEntities(ct);
  s_vec<ecs::Entity*> out;

  F__LOOP(it, ents) {
    auto e = *it;
    auto cur = CC_GEC(Team,e,"n/Team");
    if (cur && cur->team == team) {
      out.push_back(e);
    }
  }

  return out;
}

//////////////////////////////////////////////////////////////////////////////
//
ecs::Entity* closestEntOnTeam(ecs::Engine *engine, ecs::Entity *ent, int team) {

  auto ourRender = CC_GEC(f::CDraw,ent,"f/CDraw");
    ecs::Entity *closestEnt= nullptr;
  float closestDist= -1;

  if (!ourRender) { return nullptr; }

  auto others = getEntsOnTeam(engine, team, "f/CDraw");
  F__LOOP(it, others) {
    auto e= *it;
    auto r2= CC_GEC(f::CDraw,e,"f/CDraw");
    auto dist= c::ccpDistance(ourRender->pos(), r2->pos());
    if (dist < closestDist || closestDist == -1) {
      closestEnt= e;
      closestDist = dist;
    }
  }

  return closestEnt;
}

//////////////////////////////////////////////////////////////////////////////
//
void createMonsters(ecs::Engine *eng, int cost, int team, int count) {
  auto engine= SCAST(GEngine*,eng);
  auto wz=cx::visRect();
  auto wb=cx::visBox();
  for (auto i = 0;  i < count; ++i) {
      ecs::Entity *e=nullptr;
    switch (cost) {
      case COST_QUIRK:
        e= engine->createQuirkMonster(team);
      break;
      case COST_ZAP:
        e= engine->createZapMonster(team);
      break;
      case COST_MUNCH:
        e= engine->createMunchMonster(team);
      break;
    }
    if (e)  {
      auto render = CC_GEC(f::CDraw,e,"f/CDraw");
      auto r= CCRANDOM_X_Y(-wz.size.height * 0.25, wz.size.height * 0.25);
      render->setPos(wb.right * 0.25, wb.cy  + r);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
ecs::Entity* playerForTeam(ecs::Engine *engine, int team) {
  auto players = getEntsOnTeam(engine, team, "n/Player");
  return players.size() > 0 ? players[0] : nullptr;
}

//////////////////////////////////////////////////////////////////////////////
//
s_vec<ecs::Entity*> entsWithinRange(ecs::Engine *engine, ecs::Entity *ent, float range, int team) {

  auto ourRender = CC_GEC(f::CDraw,ent,"f/CDraw");
  s_vec<ecs::Entity*> out;

  if (ourRender) {

    auto all= getEntsOnTeam(engine,team,"f/CDraw");
    F__LOOP(it,all) {
      auto e = *it;
      auto otherRender = CC_GEC(f::CDraw,e,"f/CDraw");
      auto dist = c::ccpDistance(ourRender->pos(), otherRender->pos());
      if (dist < range) {
        out.push_back(e);
      }
    }

  }

  return out;
}





NS_END




