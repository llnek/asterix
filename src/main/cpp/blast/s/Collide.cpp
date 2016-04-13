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

//////////////////////////////////////////////////////////////////////////////

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Collide.h"
#include "n/Player.h"
#include "n/Enemy.h"
#include "n/Blast.h"
#include "n/Missile.h"
#include "n/PowerUp.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(blast)


//////////////////////////////////////////////////////////////////////////////
//
void Collide::preamble() {
  _player= _engine->getNodes("f/CGesture")[0];
  _shared= _engine->getNodes("n/GVars")[0];
}

//////////////////////////////////////////////////////////////////////////////
//
bool Collide::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}


//////////////////////////////////////////////////////////////////////////////
//
void Collide::process(float dt) {
  auto player= CC_GEC(Player,_player,"f/CPixie");
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto py= PCAST(f::UICObj, player);
  auto player_position = py->pos();
  auto player_radius = py->radius();
    USING_NS_CC;
  // iterate through all enemies
  c::Object *object = CC_NIL;
  CCARRAY_FOREACH(ss->enemies, object) {
    auto enemy = (Enemy*)object;
    auto uic = PCAST(c::Node,enemy);
    if(enemy) {
      auto enemy_position = uic->getPosition();
      // check with Player
      if (CIRCLE_INTERSECTS_CIRCLE(player_position, player_radius, enemy_position, ENEMY_RADIUS)) {
        // if shield is enabled, kill enemy
        if(player->getShield()) {
          enemyKilled(ss,enemy);
        }
        // else kill player...but only if enemy has finished spawning
        else if(!enemy->isSpawning) {
          player->die();
        }
      }

      // check with all blasts
      c::Object *object2 = CC_NIL;
      CCARRAY_FOREACH(ss->blasts, object2) {
        auto blast = (Blast*)object2;
        auto uic= PCAST(c::Node,blast);
        if(blast) {
          if (CIRCLE_INTERSECTS_CIRCLE(uic->getPosition(), blast->radius, enemy_position, ENEMY_RADIUS*1.5)) {
            enemyKilled(ss,enemy);
          }
        }
      }

      // check with all missiles
      object2 = NULL;
      CCARRAY_FOREACH(ss->missiles, object2) {
        auto missile = (Missile*)object2;
        auto uic= PCAST(c::Node,missile);
        if(missile) {
          if (CIRCLE_INTERSECTS_CIRCLE(uic->getPosition(), MISSILE_RADIUS, enemy_position, ENEMY_RADIUS*1.5)) {
            missile->explode();
          }
        }
      }
    }
  }

  // check if player collides with any of the power-ups
  // activate the power-up if collision is found
  object = CC_NIL;
  CCARRAY_FOREACH(ss->powerups, object) {
    auto powerup = (PowerUp*)object;
    auto uic= PCAST(c::Node,powerup);
    if (powerup && !powerup->isActive) {
      if (CIRCLE_INTERSECTS_CIRCLE(player_position, player_radius, uic->getPosition(), POWERUP_ICON_OUTER_RADIUS)) {
        powerup->activate();
      }
    }
  }
}


NS_END




