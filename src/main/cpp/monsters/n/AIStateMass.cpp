
#include "core/CCSX.h"
#include "AIState.h"
#include "AISystem.h"
#include "Entity.h"
#include "EntityManager.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(monsters)

//////////////////////////////////////////////////////////////////////////////
//
void AIStateMass::enter() {
  cx::sfxPlay("mass");
}

//////////////////////////////////////////////////////////////////////////////
//
void AIStateMass::update(not_null<ecs::Node*> node) {

  auto player = CC_GNF(Player,node,"player");
  auto team = CC_GNF(Team,node,"team");
  auto ai = CC_GNF(AI,node,"ai");

  auto enemies = entitiesWithinRange(200, OPPOSITE_TEAM(team->team));
  if (enemies.size() > 0) {
    system->changeStateForEntity(entity, new AIStateDefend());
    return;
  } else if (system->aiTotalValue + player->coins >= COST_MUNCH + COST_ZAP * 2) {
    system->changeStateForEntity(entity, new AIStateCounter());
    return;
  }

  player->attacking = false;
}


NS_END


