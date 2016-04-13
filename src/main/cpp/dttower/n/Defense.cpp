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
#include "Defense.h"
#include "Enemy.h"
#include "C.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(dttower)

//////////////////////////////////////////////////////////////////////////////
//
owner<Defense*> Defense::create(GVars *ss, DefenseLevel level) {

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
  auto po= MGMS()->getPool("Enemies");
  auto ps= po->ls();
  F__LOOP(it, ps) {
    auto p= *it;
    if (!p->status()) { continue; }
    auto e=CC_GEC(Enemy,p,"f/CPixie");
    if (detectEnemy(e)) {
      attackEnemy((ecs::Node*)p);
      break;
    }
  }

}

//////////////////////////////////////////////////////////////////////////////
//
bool Defense::detectEnemy(Enemy *e) {

  auto defenseRadius= 1.5 * CC_CSZ(this).width;
  auto enemyRadius= CC_CSZ(e).width;
  auto enemyPos= e->getPosition();
  auto pos= this->getPosition();
  auto distanceX = pos.x - enemyPos.x;
  auto distanceY = pos.y - enemyPos.y;
  auto distance = sqrt(distanceX * distanceX + distanceY * distanceY);

  return (distance <= defenseRadius + enemyRadius);
}

//////////////////////////////////////////////////////////////////////////////
//
void Defense::attackEnemy(ecs::Node *node) {

  auto bullet = cx::reifySprite("bullet.png");
  auto e=CC_GEC(Enemy,node,"f/CPixie");
  auto pos= getPosition();

  MGML()->addAtlasItem("game-pics", bullet, 2);
  bullet->setPosition(pos.x, pos.y);
  bullet->runAction(
      c::Sequence::create(
        c::MoveTo::create(0.2, e->getPosition()),
        c::CallFunc::create([=]() {
          this->checkEnemy(node,bullet);
        }),
        c::RemoveSelf::create(true),
        CC_NIL));
}

//////////////////////////////////////////////////////////////////////////////
//
void Defense::checkEnemy(ecs::Node *node, c::Sprite *bullet) {
  auto h=CC_GEC(f::CHealth,node,"f/CHealth");
  auto e=CC_GEC(Enemy,node,"f/CPixie");

  if (cx::collideN(e,bullet)) {
    h->hurt(attackPoints);
  }

  if (!h->alive()) {
    cx::hibernate(node);
  }

}


NS_END



