

#include "core/CCSX.h"
#include "AIState.h"
#include "AISystem.h"
#include "Entity.h"
#include "EntityManager.h"


NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(monsters)
//////////////////////////////////////////////////////////////////////////////
//
void AIStateDefend::enter() {
  cx::sfxPlay("defend");
}

//////////////////////////////////////////////////////////////////////////////
//
void AIStateDefend::update(not_null<ecs::Node*> node) {

  auto player = CC_GNF(Player,node,"player");
  auto team = CC_GNF(Team,node,"team");
  auto ai = CC_GNF(AI,node,"ai");

  auto enemies = entitiesWithinRange(200, OPPOSITE_TEAM(team->team));
  if (enemies.size() == 0) {
    system->changeStateForEntity(entity, new AIStateRush());
    return;
  }

  player->attacking = false;

  if (system->aiQuirkValue < system->humanZapValue &&
      player->coins > COST_QUIRK) {
    system->spawnQuirkForEntity(entity);
  }
  else if (system->aiZapValue < system->humanMunchValue &&
           player->coins > COST_ZAP) {
    system->spawnZapForEntity(entity);
  }
  else if (system->aiMunchValue < system->humanQuirkValue &&
           player->coins > COST_MUNCH) {
    system->spawnMunchForEntity(entity);
  }
  else if (system->humanTotalValue == 0) {
    while (player->coins > COST_ZAP + COST_QUIRK) {
      system->spawnQuirkForEntity(entity);
      system->spawnZapForEntity(entity);
    }
  }
}



NS_END


