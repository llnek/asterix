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
#include "AILogic.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(monsters)

//////////////////////////////////////////////////////////////////////////////
//
void AI::preamble() {
  auto out= engine->getEntities("n/Automa");
  _enemy=out[0];
}

//////////////////////////////////////////////////////////////////////////////
//
bool AI::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::process(float dt) {
  auto aiTeam = CC_GEC(Team,_enemy,"n/Team");
  auto ai = CC_GEC(Automa,_enemy,"n/Automa");

  this->humanQuirkValue = 0;
  this->humanZapValue = 0;
  this->humanMunchValue = 0;

  //get human soldiers
  auto mons = getEntsOnTeam(engine, OTHER_TEAM(aiTeam->team), "n/Monster");
  F__LOOP(it,mons) {
    auto m= *it;
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

  mons= getEntsOnTeam(engine, aiTeam->team, "n/Monster");
  F__LOOP(it, mons) {
    auto m= *it;
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
  ai->state->update(aiEntity,this);
}

//////////////////////////////////////////////////////////////////////////////
//
void AILogic::changeStateForEntity(Entity *ent, AIState *state) {
  auto ai = CC_GEC(Automa,ent,"n/Automa");
  if (ai) {
    ai->replaceState(state);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void AILogic::spawnMonster(Entity *e, int cost, int count) {

  auto player = CC_GEC(Stash,e,"n/Stash");
  ecs::Entity *m;
  auto team=2;
  auto wz= cx::visRect();
  auto wb= cx::visBox();

  if (!player || player->coins < cost) {
  return; } else {
    player->coins -= cost;
  }

  for (auto i = 0; i < count; ++i) {
    switch (cost) {
      case COST_QUIRK:
        m= engine->createQuirkMonster(team);
      break;
      case COST_ZAP:
        m= engine->createZapMonster(team);
      break;
      case COST_MUNCH:
        m= engine->createMunchMonster(team);
      break;
      default:
      throw "bad monster type!";
    }
    auto render = CC_GEC(f::CDraw,m,"f/CDraw");
    auto r= CCRANDOM_X_Y(-wz.size.height * 0.25, wz.size.height * 0.25);
    render->setPos(wb.right * 0.75, wb.cy + r);
  }
  cx::sfxPlay("spawn");
}

//////////////////////////////////////////////////////////////////////////////
//
void AILogic::spawnQuirkForEntity(Entity *e) {
  spawnMonster(e,COST_QUIRK,2);
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::spawnZapForEntity(GEngine *engine, Entity *entity) {
  spawnMonster(e,COST_ZAP,1);
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::spawnMunchForEntity(GEngine *engine, Entity *ent) {
  spawnMonster(e,COST_MUNCH,1);
}


NS_END



