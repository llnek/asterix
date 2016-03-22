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
#include "Defense.h"
#include "Enemy.h"
#include "C.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(dttower)

//////////////////////////////////////////////////////////////////////////////
//
owner<Defense*> Defense::create(GVars *ss, DefenseLevel level, const c::Vec2 &pos) {

  auto rc= mc_new(Defense);
  sstr fn;
  int pts=1;
  switch (level) {
    case levelOne:
      fn = "defense_level_1.png";
      pts = 1;
    break;

    case levelTwo:
      fn = "defense_level_2.png";
      pts=2;
    break;
  }

  rc->initWithSpriteFrameName(fn);
  rc->autorelease();

  rc->defenseLevel = level;
  rc->attackPoints = pts;
  rc->ss= ss;

  return rc;
}

//////////////////////////////////////////////////////////////////////////////
//
void Defense::update(float dt) {

  auto pos= getPosition();
  auto sz= CC_CSZ(this);

  if (!this->enemyInRange ) {

    F__LOOP(it, ss->enemies) {
      auto e= *it;
      if (detectEnemyWithDefenseAtPos(
            pos,
            1.5*sz.width,
            e->getPosition(),
            CC_CSZ(e).width)) {
        enemyInRange = e;
        e->addAttackingDefense(this);
        schedule(CC_SCHEDULE_SELECTOR(Defense::attackEnemy),0.5);
        break;
      }
    }

  }
  else
  if (!detectEnemyWithDefenseAtPos(pos,
        1.5*sz.width,
        enemyInRange->getPosition(),
        CC_CSZ(enemyInRange).width)) {
    enemyOutOfRange();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
bool Defense::detectEnemyWithDefenseAtPos(
    const c::Vec2 &pos,
    float defenseRadius,
    const c::Vec2 &enemyPosition,
    float enemyRadius) {

  auto distanceX = pos.x - enemyPosition.x;
  auto distanceY = pos.y - enemyPosition.y;
  auto distance = sqrt(distanceX * distanceX + distanceY * distanceY);

  return (distance <= defenseRadius + enemyRadius);
}

//////////////////////////////////////////////////////////////////////////////
//
void Defense::attackEnemy(float dt) {
  if (!enemyInRange ) {
  return; }

  auto bullet = cx::reifySprite("bullet.png");
  auto pos= getPosition();

  bullet->setPosition(pos.x, pos.y);
  MGML()->addAtlasItem("game-pics", bullet, 2);

  bullet->runAction(
      c::Sequence::create(
        c::MoveTo::create(0.2, enemyInRange->getPosition()),
        c::CallFunc::create([=]() {
          enemyInRange->lifePoints -= attackPoints;
        }),
        c::RemoveSelf::create(true),
        CC_NIL));
}

//////////////////////////////////////////////////////////////////////////////
//
void Defense::enemyOutOfRange() {
  unschedule(CC_SCHEDULE_SELECTOR(Defense::attackEnemy));
  enemyInRange->outOfRangeFromDefense(this);
  enemyInRange = CC_NIL;
}

//////////////////////////////////////////////////////////////////////////////
//
void Defense::enemyKilled() {
  auto it= s::find(ss->enemies.begin(),ss->enemies.end(), enemyInRange);
  if (it != ss->enemies.end()) {
    ss->enemies.erase(it);
  }
}


NS_END



