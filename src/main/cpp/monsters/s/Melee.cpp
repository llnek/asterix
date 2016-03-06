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
#include "Collide.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(monsters)


//////////////////////////////////////////////////////////////////////////////
//
void MeleeLogic::preamble() {
  shared=engine->getNodeList(SharedNode().typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
bool MeleeLogic::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}


//////////////////////////////////////////////////////////////////////////////
//
void MeleeLogic::process(float dt) {

  auto ents = engine->getEntities(
      s_vec<ecs::COMType>{"n/Team","n/Melee","f/CDraw"});

  F__LOOP(it,ents) {
    auto e = *it;
    auto render = CC_GEC(f::CDraw,e,"f/CDraw");
    auto melee = CC_GEC(Melee,e,"n/Melee");
    auto team = CC_GEC(Team,e,"n/Team");
    auto aoeDamageCaused = false;
    auto enemies = getEntsOnTeam(engine,OTHER_TEAM(team->team),"f/CDraw");
    F__LOOP(it2,enemies) {
      auto enemy= *it2;
      auto enemyRender = CC_GEC(f::CDraw,enemy,"f/CDraw");
      auto enemyHealth = CC_GEC(f::CHealth,enemy,"f/CHealth");
      if (!enemyHealth) { continue; }

      if (render->bbox().rectIntersectsRect(enemyRender->bbox())) {
        if (cx::timeInMillis() - melee->lastDamageTime > melee->damageRate) {

          cx::sfxPlay(melee->sound);

          if (melee->aoe) {
            aoeDamageCaused = true;
          } else {
            melee->lastDamageTime = cx::timeInMillis();
          }
          enemyHealth->curHp -= melee->damage;
          if (enemyHealth->curHp < 0) {
            enemyHealth->curHp = 0;
          }
          if (melee->selfDie) {
            render->removeInnerNode();
            engine->purgeEntity(e);
          }
        }
      }
    }

    if (aoeDamageCaused) {
      melee->lastDamageTime = cx::timeInMillis();
    }
  }
}


NS_END




