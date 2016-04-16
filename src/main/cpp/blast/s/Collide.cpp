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
  auto player_position = player->pos();
  auto player_radius = player->radius();
  USING_NS_CC;
  // iterate through all enemies
  c::Object *object = CC_NIL;
  CCARRAY_FOREACH(ss->enemies, object) {
    auto enemy = (Enemy*)object;
    if(enemy) {
      auto enemy_position = enemy->getPosition();
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
        if(blast) {
          if (CIRCLE_INTERSECTS_CIRCLE(blast->getPosition(), blast->radius, enemy_position, ENEMY_RADIUS*1.5)) {
            enemyKilled(ss,enemy);
          }
        }
      }

      // check with all missiles
      object2 = NULL;
      CCARRAY_FOREACH(ss->missiles, object2) {
        auto missile = (Missile*)object2;
        if(missile) {
          if (CIRCLE_INTERSECTS_CIRCLE(missile->getPosition(), MISSILE_RADIUS, enemy_position, ENEMY_RADIUS*1.5)) {
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
    if (powerup && !powerup->isActive) {
      if (CIRCLE_INTERSECTS_CIRCLE(player_position, player_radius, powerup->getPosition(), POWERUP_ICON_OUTER_RADIUS)) {
        powerup->activate();
      }
    }
  }
}


NS_END




