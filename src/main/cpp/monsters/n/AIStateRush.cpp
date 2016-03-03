
#include "AISystem.h"
#include "Entity.h"
#include "EntityManager.h"
#include "AIState.h"

NS_BEGIN(monsters)
//////////////////////////////////////////////////////////////////////////////
//
void AIStateRush::enter() {
  cx::sfxPlay("attack");
}

//////////////////////////////////////////////////////////////////////////////
//
void AIStateRush::update(not_null<ecs::Node*> node) {

  auto player = CC_GNF(Player,node,"player");
  auto team = CC_GNF(Team,node,"team");
  auto ai = CC_GNF(AI,node,"ai");

  //assert not nulls
  auto enemies = entitiesWithinRange(200, OPPOSITE_TEAM(team->team));
  if (enemies.size() > 0) {
    system->changeStateForEntity(entity, new AIStateDefend());
    return;
  } else if (system.aiTotalValue == 0) {
    system->changeStateForEntity(entity, new AIStateMass());
    return;
  }

  player->attacking = true;

  if (system->aiTotalValue > system->humanTotalValue) {
    system->spawnQuirkForEntity(entity);
  }
}



NS_END


