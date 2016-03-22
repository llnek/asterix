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

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Enemy.h"
#include "PathStep.h"
#include "Defense.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(dttower)

//////////////////////////////////////////////////////////////////////////////
//
owner<Enemy*> Enemy::create(GVars *ss, not_null<PathStep*> step) {

  auto rc= mc_new(Enemy);

  rc->initWithSpriteFrameName("enemy.png");
  rc->autorelease();
  rc->setPosition(step->getPosition());
  rc->pathStep = step;
  rc->ss=ss;

  return rc;
}

//////////////////////////////////////////////////////////////////////////////
//
void Enemy::update(float dt) {

  auto ppos= pathStep->next->getPosition();
  auto pos= getPosition();
  auto speed= SPEED;

  auto dist= c::ccpDistance(pos, ppos);
  if (dist < speed ) {
    speed = dist;
  }

  if (dist > 0) {
    if (pos.y > ppos.y) {
      // down
      setPosition(pos.x, pos.y - speed);
    } else if (pos.y < ppos.y) {
      // up
      setPosition(pos.x, pos.y + speed);
    } else if (pos.x > ppos.x) {
      // left
      setPosition(pos.x - speed, pos.y);
    } else if (pos.x < ppos.x) {
        // right
      setPosition(pos.x + speed, pos.y);
    }
  } else if (pathStep->next->next != CC_NIL) {
      // Look for next step
    pathStep = pathStep->next;
  } else {
    SENDMSGEX("/game/player/enemyreachedtower", this);
    removeEnemyFromScene();
  }

  if (lifePoints <= 0) {
    removeEnemyFromScene();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Enemy::addAttackingDefense(not_null<Defense*> d) {
  attackingDefenses.push_back(d.get());
}

//////////////////////////////////////////////////////////////////////////////
//
void Enemy::outOfRangeFromDefense(Defense *attacker) {
  if (attacker) {
    auto it= s::find(attackingDefenses.begin(),
        attackingDefenses.end(),attacker);
    if (it != attackingDefenses.end()) {
      attackingDefenses.erase(it);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Enemy::removeEnemyFromScene() {
  F__LOOP(it,attackingDefenses) {
    auto d = *it;
    d->enemyKilled();
  }
  removeFromParent();
}


NS_END


