
#include "AISystem.h"
#include "Entity.h"
#include "EntityManager.h"
#include "EntityFactory.h"
#include "AIState.h"


//////////////////////////////////////////////////////////////////////////////
//
void AISystem::preamble() {
  robots=engine->getNodeList();
}

//////////////////////////////////////////////////////////////////////////////
//
bool AISystem::update(float dt) {

  //aiEntity
  auto node= robots->head;

  if (! node) { return true;}

  auto team= CC_GNF(Team, node, "team");
  auto ai= CC_GNF(AI, node, "ai");

  if (!team || !ai) { return true; }

  humanQuirkValue = 0;
  humanZapValue = 0;
  humanMunchValue = 0;

  auto humanMonsters = node->getAllEntitiesOnTeam(
      OPPOSITE_TEAM(team->team),
      "n/Monster");

  F__LOOP(it, humanMonsters) {
    auto humanMonster = *it;
    MonsterComponent *monster = humanMonster->monster;
    if (monster->type == MonsterTypeQuirk) {
      humanQuirkValue += COST_QUIRK;
    } else if (monster->type == MonsterTypeZap) {
      humanZapValue += COST_ZAP;
    } else if (monster->type == MonsterTypeMunch) {
      humanMunchValue += COST_MUNCH;
    }
  }

  humanTotalValue = humanQuirkValue + humanZapValue + humanMunchValue;

  aiQuirkValue = 0;
  aiZapValue = 0;
  aiMunchValue = 0;

  auto aiMonsters = aiEntity->getAllEntitiesOnTeam(team->team, "n/Monster");
  F__LOOP(it, aiMonsters) {
    auto aiMonster = *it;
    MonsterComponent *monster = aiMonster->monster;
    if (monster->type == MonsterTypeQuirk) {
      aiQuirkValue += COST_QUIRK;
    } else if (monster->type == MonsterTypeZap) {
      aiZapValue += COST_ZAP;
    } else if (monster->type == MonsterTypeMunch) {
      aiMunchValue += COST_MUNCH;
    }
  }
  aiTotalValue = aiQuirkValue + aiZapValue + aiMunchValue;
  ai->state->update(node, this);
}

//////////////////////////////////////////////////////////////////////////////
//
void AISystem::changeStateForEntity(not_null<ecs::Node*> node,
    AIState *state) {

  auto ai= CC_GNF(Robot, node, "ai");
  ai->state->exit();
  delete ai->state;
  ai->state = state;
  ai->state->enter();
}

//////////////////////////////////////////////////////////////////////////////
//
void AISystem::spawnQuirkForEntity(not_null<ecs::Node*> node) {

  auto player = CC_GNF(Player, node, "player");
  auto wz= cx::visRect();
  auto wb= cx::visBox();

  if (player->coins < COST_QUIRK) { return; }
  player->coins -= COST_QUIRK;

  cx::sfxPlay("spawn");

  for (auto i = 0; i < 2; ++i) {
    auto monster = engine->createQuirkMonsterWithTeam(2);
    auto render = monster->get("n/Render");
    if (render) {
      auto randomOffset = CCRANDOM_X_Y(-wz.size.height * 0.25, wz.size.height * 0.25);
      render->setPos(wb.right * 0.75, wb.cy + randomOffset);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void AISystem::spawnZapForEntity(not_null<ecs::Node*> node) {

  auto player = CC_GNF(Player, node, "player");
  auto wz= cx::visRect();
  auto wb= cx::visBox();

  if (player->coins < COST_ZAP) { return; }
  player->coins -= COST_ZAP;

  cx::sfxPlay("spawn");

  auto monster = engine->createZapMonsterWithTeam(2);
  auto render = monster->get("n/Render");
  if (render) {
    auto h = CCRANDOM_X_Y(-wz.size.height * 0.25, wz.size.height * 0.25);
    render->setPos(wb.right * 0.75, wb.cy + h);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void AISystem::spawnMunchForEntity(not_null<ecs::Node*> node) {

  auto player = CC_GNF(Player, node, "player");
  auto wz= cx::visRect();
  auto wb= cx::visBox();

  if (player->coins < COST_MUNCH) { return; }
  player->coins -= COST_MUNCH;

  cx::sfxPlay("spawn");

  auto monster = engine->createMunchMonsterWithTeam(2);
  auto render = monster->get("n/Render");
  if (render) {
    auto h = CCRANDOM_X_Y(-wz.size.height * 0.25, wz.size.height * 0.25);
    render->setPos(wb.right * 0.75, wb.cy + h);
  }
}


NS_END


