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

#include "core/CCSX.h"
#include "GEngine.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(monsters)

//////////////////////////////////////////////////////////////////////////////
//
s_vec<ecs::Node*> getEntsOnTeam(
    ecs::Engine *engine, int team,  const ecs::COMType &ct) {

  auto ents= engine->getNodes(ct);
  s_vec<ecs::Node*> out;

  F__LOOP(it, ents) {
    auto &e= *it;
    auto cur = CC_GEC(Team,e,"n/Team");
    if (cur && cur->team == team) {
      out.push_back(e);
    }
  }

  return out;
}

//////////////////////////////////////////////////////////////////////////////
//
ecs::Node* closestEntOnTeam(
    ecs::Engine *engine, ecs::Node *ent, int team) {

  auto me = CC_GEC(f::CPixie,ent,"f/CPixie");
  ecs::Node *closestEnt= CC_NIL;;
  float closestDist= -1;

  if (!me) { return CC_NIL; }

  auto others = getEntsOnTeam(engine, team, "f/CPixie");
  auto pos= me->pos();
  F__LOOP(it, others) {
    auto &e= *it;
    auto r2= CC_GEC(f::CPixie,e,"f/CPixie");
    auto dist= c::ccpDistance(pos, r2->pos());
    if (dist < closestDist ||
        closestDist == -1) {
      closestDist = dist;
      closestEnt= e;
    }
  }

  return closestEnt;
}

//////////////////////////////////////////////////////////////////////////////
//
static void cfgMonster(ecs::Node *node) {
  auto c = CC_GEC(f::CPixie,node,"f/CPixie");
  auto wz=cx::visSize();
  auto wb=cx::visBox();
  auto r= CCRANDOM_X_Y(-CC_ZH(wz) * 0.25, CC_ZH(wz) * 0.25);
  CC_POS2(c, wb.right * 0.25, wb.cy  + r);
}

//////////////////////////////////////////////////////////////////////////////
//
void createMonsters(ecs::Engine *eng, int cost, int team, int count) {
  auto engine= SCAST(GEngine*,eng);
  for (auto i = 0;  i < count; ++i) {
    switch (cost) {
      case COST_QUIRK:
        cfgMonster( engine->createQuirkMonster(team));
      break;
      case COST_ZAP:
        cfgMonster( engine->createZapMonster(team));
      break;
      case COST_MUNCH:
        cfgMonster( engine->createMunchMonster(team));
      break;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
ecs::Node* playerForTeam(ecs::Engine *engine, int team) {
  auto players = getEntsOnTeam(engine, team, "n/Player");
  return players.size() > 0 ? players[0] : CC_NIL;
}

//////////////////////////////////////////////////////////////////////////////
//
s_vec<ecs::Node*> entsWithinRange(ecs::Engine *engine, ecs::Node *ent, float range, int team) {

  auto me = CC_GEC(f::CPixie,ent,"f/CPixie");
  s_vec<ecs::Node*> out;

  if (me) {
    auto all= getEntsOnTeam(engine,team,"f/CPixie");
    auto pos= me->pos();
    F__LOOP(it,all) {
      auto &e = *it;
      auto other= CC_GEC(f::CPixie,e,"f/CPixie");
      auto dist = c::ccpDistance(pos, other->pos());
      if (dist < range) {
        out.push_back(e);
      }
    }
  }

  return out;
}





NS_END




