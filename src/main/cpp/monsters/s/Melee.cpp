// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Melee.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(monsters)


//////////////////////////////////////////////////////////////////////////////
//
void MeleeLogic::preamble() {
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

  auto ents = _engine->getNodes(
      s_vec<ecs::COMType>{"n/Team","f/CMelee","f/CPixie"});

  F__LOOP(it,ents) {
    auto &e = *it;
    auto render = CC_GEC(f::CPixie,e,"f/CPixie");
    auto melee = CC_GEC(Melee,e,"f/CMelee");
    auto team = CC_GEC(Team,e,"n/Team");
    auto aoeDamageCaused = false;
    auto enemies = getEntsOnTeam(_engine,OTHER_TEAM(team->team),"f/CPixie");
    F__LOOP(it2,enemies) {
      auto &enemy= *it2;
      auto enemyHealth = CC_GEC(f::CHealth,enemy,"f/CHealth");
      auto enemyRender = CC_GEC(f::CPixie,enemy,"f/CPixie");
      if (!enemyHealth) { continue; }

      if (render->bbox().intersectsRect(enemyRender->bbox())) {
        if (cx::timeInMillis() - melee->lastDamageTime > melee->damageRate) {

          cx::sfxPlay(melee->sound);

          if (melee->aoe) {
            aoeDamageCaused = true;
          } else {
            melee->lastDamageTime = cx::timeInMillis();
          }
          enemyHealth->curHP -= melee->damage;
          if (enemyHealth->curHP < 0) {
            enemyHealth->curHP = 0;
          }
          if (melee->selfDie) {
            render->removeFromParent();
            _engine->purgeNode(e);
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




