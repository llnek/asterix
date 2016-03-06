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
#include "AIState.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(monsters)

//////////////////////////////////////////////////////////////////////////////
//
void AI::preamble() {

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

  auto ents = engine->getEntities(
      s_vec<ecs::COMType>{"n/Automa","n/Team"} );

  if  (ents.size() == 0) {
  return; }

  auto aiEntity = ents[0];
  auto aiTeam = (CTeam*)aiEntity->get("n/Team");
  auto ai = (CAutoma*)aiEntity->get("n/Automa");

  this->humanQuirkValue = 0;
  this->humanZapValue = 0;
  this->humanMunchValue = 0;

  auto mons = getEntsOnTeam(engine, OTHER_TEAM(aiTeam.team), "n/Monster");
  F__LOOP(it,mons) {
    auto m= *it;
    auto c= (CMonster*)m->get("n/Monster");
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
    auto c= (CMonster*) m->get("n/Monster");
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
void AI::changeStateForEntity(Entity *entity, AIState *state) {
  auto ai = entity->get("n/Automa");
  if (!ai) return;
  //ai->state->exit();
  //ai.state = state;
  //[ai.state enter];
  ai->replaceState(state);
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::spawnQuirkForEntity(GEngine *engine, Entity *entity) {

  auto player = entity->get("n/Score");
  auto wz= cx::visRect();
  auto wb= cx::visBox();

  if (!player || player->coins < COST_QUIRK) { return; } else {
    player->coins -= COST_QUIRK;
  }

  cx::sfxPlay("spawn");

  for (auto i = 0; i < 2; ++i) {
    auto *m= engine->createQuirkMonster(2);
    auto *render = (f::cmRender*)m->get("f/CmRender");
    if (render) {
      auto r= CCRANDOM_X_Y(-wz.size.height * 0.25, wz.size.height * 0.25);
      render->setPos(wb.right * 0.75, wb.cy + r);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::spawnZapForEntity(GEngine *engine, Entity *entity) {

  auto player = entity->get("n/Score");
  auto wz= cx::visRect();
  auto wb= cx::visBox();

  if (!player || player->coins < COST_ZAP) { return; } else {
    player->coins -= COST_ZAP;
  }

  cx::sfxPLay("spawn");

  auto m= engine->createZapMonster(2);
  auto render = (f::CmRender*)m->get("f/CmRender");
  if (render) {
    auto r= CCRANDOM_X_Y(-wz.size.height * 0.25, wz.size.height * 0.25);
    render->setPos(wb.right * 0.75, wb.cy + r);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::spawnMunchForEntity(GEngine *engine, Entity *entity) {

  auto player = entity->get("n/Score");
  auto wz= cx::visRect();
  auto wb= cx::visBox();

  if (!player || player->coins < COST_MUNCH) { return; } else {
    player->coins -= COST_MUNCH;
  }

  cx::sfxPlay("spawn");

  auto m= engine->createMunchMonster(2);
  auto render = (f::CmRender*)m->get("f/CmRender");
  if (render) {
    auto r= CCRANDOM_X_Y(-wz.size.height * 0.25, wz.size.height * 0.25);
    render->setPos(wb.right * 0.75, wb.cy  + r);
  }
}

NS_END



