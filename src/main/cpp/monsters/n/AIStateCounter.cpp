
#include "core/CCSX.h"
#include "AIState.h"
#include "AISystem.h"
#include "Entity.h"
#include "EntityManager.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(monsters)

//////////////////////////////////////////////////////////////////////////////
//
void AIStateCounter::update(not_null<ecs::Node*> node) {

  auto player = CC_GNF(Player,node,"player");
  auto team = CC_GNF(Team,node,"team");
  auto ai = CC_GNF(AI,node,"ai");
  auto enemies = entitiesWithinRange(200, OPPOSITE_TEAM(team->team));

  if (enemies.size() > 0) {

    while (player->coins > COST_QUIRK) {
      if (system->aiQuirkValue < system->humanZapValue &&
          player->coins > COST_QUIRK) {
        system->spawnQuirkForEntity(node);
      }
      else if (system->aiZapValue < system->humanMunchValue &&
               player->coins > COST_ZAP) {
        system->spawnZapForEntity(node);
      }
      else if (system->aiMunchValue < system->humanQuirkValue &&
               player->coins > COST_MUNCH) {
        system->spawnMunchForEntity(node);
      } else {
        system->spawnQuirkForEntity(node);
      }
    }

    system->changeStateForEntity(entity, new AIStateDefend());
  }
}


NS_END



