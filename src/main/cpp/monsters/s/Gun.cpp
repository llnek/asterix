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
#include "Gun.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(monsters)

//////////////////////////////////////////////////////////////////////////////
//
void GunLogic::preamble() {

}

//////////////////////////////////////////////////////////////////////////////
//
bool GunLogic::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

static float laserPointsPerSecond = 100;
static float laserDistance = 1000;
static float WIGGLE_ROOM = 5;
//////////////////////////////////////////////////////////////////////////////
//
void GunLogic::process(float dt) {
  auto ents = engine->getEntities(
      s_vec<ecs::COMType>{"n/Team", "n/Gun", "f/CDraw"});
  F__LOOP(it,ents) {
    auto e= *it;
    auto render = CC_GEC(f::CDraw,e,"f/CDraw");
    auto team = CC_GEC(Team,e,"n/Team");
    auto gun = CC_GEC(Gun,e,"n/Gun");
    auto enemy = closestEntOnTeam(engine,e, OTHER_TEAM(team->team));
    if (!enemy) { return; }

    auto enemyRender = CC_GEC(f::CDraw,enemy,"f/CDraw");
    auto dist = c::ccpDistance(render->pos(), enemyRender->pos());

    if (abs(dist) <= (gun->range + WIGGLE_ROOM) &&
        (cx::timeInMillis() - gun->lastDamageTime) > gun->damageRate) {

      cx::sfxPlay(gun->sound);
      gun->lastDamageTime = cx::timeInMillis();

      auto laser = engine->createLaser(team->team);
      auto laserRender = CC_GEC(f::CDraw,laser,"f/CDraw");
      auto laserMelee = CC_GEC(Melee,laser,"n/Melee");

      laserRender->node->setPosition(render->pos());
      laserMelee->damage = gun->damage;

      auto direction = c::ccpNormalize(
          c::ccpSub(enemyRender->pos(), render->pos()));

      auto target = c::ccpMult(direction, laserDistance);
      auto duration = laserDistance / laserPointsPerSecond;

      laserRender->node->setRotation( -1 * CC_RADIANS_TO_DEGREES(c::ccpToAngle(direction)));
      laserRender->node->setZOrder(1);

      laserRender->node->runAction(
         c::Sequence::create(
          c::MoveBy::create(duration, target),
          c::CallFunc::create([=](){
            laserRender->releaseInnerNode();
            engine->purgeEntity(laser);
            }),
          c::RemoveSelf::create(),
          CC_NIL));
    }
  }
}



NS_END

