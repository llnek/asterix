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

//////////////////////////////////////////////////////////////////////////////

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "AI.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(monsters)

//////////////////////////////////////////////////////////////////////////////
//
void AILogic::preamble() {
  _enemy= _engine->getNodes("f/CAutoma")[0];
}

//////////////////////////////////////////////////////////////////////////////
//
bool AILogic::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void AILogic::process(float dt) {
  auto ai = CC_GEC(Automa,_enemy,"f/CAutoma");
  auto aiTeam = CC_GEC(Team,_enemy,"n/Team");

  this->humanMunchValue = 0;
  this->humanZapValue = 0;
  this->humanQuirkValue = 0;

  //get human soldiers
  auto mons = getEntsOnTeam(_engine, OTHER_TEAM(aiTeam->team), "n/Monster");
  F__LOOP(it,mons) {
    auto &m= *it;
    auto c= CC_GEC(Monster,m,"n/Monster");
    if (c->type == eMonsterTypeQuirk) {
      this->humanQuirkValue += COST_QUIRK;
    } else if (c->type == eMonsterTypeZap) {
      this->humanZapValue += COST_ZAP;
    } else if (c->type == eMonsterTypeMunch) {
      this->humanMunchValue += COST_MUNCH;
    }
  }

  humanTotalValue = humanQuirkValue + humanZapValue + humanMunchValue;
  this->aiQuirkValue = 0;
  this->aiZapValue = 0;
  this->aiMunchValue = 0;

  mons= getEntsOnTeam(_engine, aiTeam->team, "n/Monster");
  F__LOOP(it, mons) {
    auto &m= *it;
    auto c= CC_GEC(Monster,m,"n/Monster");
    if (c->type == eMonsterTypeQuirk) {
      this->aiQuirkValue += COST_QUIRK;
    } else if (c->type == eMonsterTypeZap) {
      this->aiZapValue += COST_ZAP;
    } else if (c->type == eMonsterTypeMunch) {
      this->aiMunchValue += COST_MUNCH;
    }
  }

  aiTotalValue = aiQuirkValue + aiZapValue + aiMunchValue;
  ai->state->update(_enemy,this);
}

//////////////////////////////////////////////////////////////////////////////
//
void AILogic::changeStateForEntity(ecs::Node *ent, AIState *state) {
  auto ai = CC_GEC(Automa,ent,"f/CAutoma");
  if (ai) {
    ai->replaceState(state);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
static void cfgMonster(ecs::Node *m) {
  auto render = CC_GEC(f::CPixie,m,"f/CPixie");
  auto wz= cx::visSize();
  auto wb= cx::visBox();
  auto r= CCRANDOM_X_Y(-CC_ZH(wz) * 0.25, CC_ZH(wz) * 0.25);
  CC_POS2(render, wb.right * 0.75, wb.cy + r);
}

//////////////////////////////////////////////////////////////////////////////
//
void AILogic::spawnMonster(ecs::Node *e, int cost, int count) {

  auto player = CC_GEC(Player,e,"n/Player");
  auto eng= SCAST(GEngine*,_engine);
  ecs::Node *m;
  auto team=2;

  if (!player || player->coins < cost) {
  return; } else {
    player->coins -= cost;
  }

  for (auto i = 0; i < count; ++i) {
    switch (cost) {
      case COST_QUIRK:
        cfgMonster( eng->createQuirkMonster(team));
      break;
      case COST_ZAP:
        cfgMonster( eng->createZapMonster(team));
      break;
      case COST_MUNCH:
        cfgMonster( eng->createMunchMonster(team));
      break;
      default:
      throw "bad monster type!";
    }
  }

  cx::sfxPlay("spawn");
}

//////////////////////////////////////////////////////////////////////////////
//
void AILogic::spawnQuirkForEntity(ecs::Node *e) {
  spawnMonster(e,COST_QUIRK,2);
}

//////////////////////////////////////////////////////////////////////////////
//
void AILogic::spawnZapForEntity(ecs::Node *e) {
  spawnMonster(e,COST_ZAP,1);
}

//////////////////////////////////////////////////////////////////////////////
//
void AILogic::spawnMunchForEntity(ecs::Node *e) {
  spawnMonster(e,COST_MUNCH,1);
}


NS_END



