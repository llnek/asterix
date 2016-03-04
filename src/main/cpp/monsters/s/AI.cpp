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
  enemies=engine->getNodeList(EnemyNode().typeId());
  shared=engine->getNodeList(SharedNode().typeId());
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

  auto node = enemies->head;
  if (ENP(node)) {
  return; }

  auto aiTeam = CC_GNF(Team,node,"team");
  auto ai = CC_GNF(Automa,node,"ai");
  auto otherTeam= OPPOSITE_TEAM(aiTeam->team);

  this->humanQuirkValue = 0;
  this->humanZapValue = 0;
  this->humanMunchValue = 0;

  auto po= MGMS()->getPool(s::to_string(otherTeam));
  auto &hs=po->list();
  F__LOOP(it,hs) {
    auto m= *it;
    if (m->status) {
      if (m->type == eMonsterTypeQuirk) {
        this->humanQuirkValue += COST_QUIRK;
      } else if (m->type == eMonsterTypeZap) {
        this->humanZapValue += COST_ZAP;
      } else if (m->type == eMonsterTypeMunch) {
        this->humanMunchValue += COST_MUNCH;
      }
    }
  }

  this->humanTotalValue =
    this->humanQuirkValue +
    this->humanZapValue + this->humanMunchValue;

  this->aiQuirkValue = 0;
  this->aiZapValue = 0;
  this->aiMunchValue = 0;

  po=MGMS()->getPool(s::to_string(aiTeam->team));
  auto &es=po->list();
  F__LOOP(it,es) {
    auto m= *it;
    if (m->status) {
      if (m->type == eMonsterTypeQuirk) {
        this->aiQuirkValue += COST_QUIRK;
      } else if (m->type == eMonsterTypeZap) {
        this->aiZapValue += COST_ZAP;
      } else if (m->type == eMonsterTypeMunch) {
        this->aiMunchValue += COST_MUNCH;
      }
    }
  }

  this->aiTotalValue =
    this->aiQuirkValue +
    this->aiZapValue + this->aiMunchValue;

  ai->state->update(node,this);
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::changeStateForEntity(
    not_null<ecs::Node*> node,
    not_null<AIState*> s) {

  auto ai= CC_GNF(Automa,node.get(),"ai");
  ai->clear();
  ai->set(s);
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::spawnQuirkForEntity(not_null<ecs::Node*> node) {

  auto player= CC_GNF(Player,node,"player");
  auto wb=cx::visBox();

  if (player->coins < COST_QUIRK) {
  return; }

  player->coins -= COST_QUIRK;
  cx::sfxPlay("spawn");

  for (auto i = 0; i < 2; ++i) {
    auto m= engine->createQuirkMonster(2);
    auto render = (Render*)m->get("n/Render");
    auto r= CCRANDOM_X_Y(-wb.top * 0.25, wb.top * 0.25);
    render->setPos(wb.right * 0.75, wb.cy + r);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::spawnZapForEntity(not_null<ecs::Node*> node) {

  auto player= CC_GNF(Player,node,"player");
  auto wb=cx::visBox();

  if (player->coins < COST_ZAP) {
  return; }
  player->coins -= COST_ZAP;

  cx::sfxPlay("spawn");

  auto m= engine->createZapMonster(2);
  auto render = (Render*)m->get("n/Render");
  auto r= CCRANDOM_X_Y(-wb.top * 0.25, wb.top * 0.25);
  render->setPos(wb.right * 0.75, wb.cy + r);
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::spawnMunchForEntity(not_null<ecs::Node*> node) {

  auto player= CC_GNF(Player,node,"player");
  auto wb=cx::visBox();

  if (player->coins < COST_MUNCH) {
  return; }

  player->coins -= COST_MUNCH;
  cx::sfxPlay("spawn");

  auto m= engine->createMunchMonster(2);
  auto render = (Render*)m->get("n/Render");
  auto r= CCRANDOM_X_Y(-wb.top * 0.25, wb.top * 0.25);
  render->setPos(wb.right * 0.75, wb.cy + r);
}





NS_END



