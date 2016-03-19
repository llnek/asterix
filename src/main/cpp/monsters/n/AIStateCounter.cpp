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

#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"
#include "ecs/Node.h"
#include "s/AI.h"
#include "AIStateCounter.h"
#include "AIStateDefend.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(monsters)

//////////////////////////////////////////////////////////////////////////////
//
void AIStateCounter::update(ecs::Node *e, AILogic *sys) {

  auto player = CC_GEC(Player,e,"n/Player");
  auto team = CC_GEC(Team,e,"n/Team");
  auto ai = CC_GEC(Automa,e,"f/CAutoma");

  if (!team || !player || !ai) {
  return; }

  auto enemies = entsWithinRange(
      sys->getEngine(),e,200, OTHER_TEAM(team->team));

  if (enemies.size() > 0) {
    while (player->coins > COST_QUIRK) {
      if (sys->aiQuirkValue < sys->humanZapValue &&
          player->coins > COST_QUIRK) {
        sys->spawnQuirkForEntity(e);
      } else if (sys->aiZapValue < sys->humanMunchValue &&
          player->coins > COST_ZAP) {
        sys->spawnZapForEntity(e);
      } else if (sys->aiMunchValue < sys->humanQuirkValue &&
          player->coins > COST_MUNCH) {
        sys->spawnMunchForEntity(e);
      } else {
        sys->spawnQuirkForEntity(e);
      }
    }

    sys->changeStateForEntity(e, mc_new(AIStateDefend));
  }
}




NS_END



