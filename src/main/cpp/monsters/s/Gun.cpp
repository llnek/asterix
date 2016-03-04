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
void GunSystem::preamble() {
  shared=engine->getNodeList(SharedNode().typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
bool GunSystem::update(float dt) {
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
void GunSystem::process(float dt) {

  auto ents= engine->getNodesWith("n/Gun");
  F__LOOP(it, ents) {
    auto e= *it;
    if (!e->status()) {
    continue; }
    auto render= e->get("n/Render");
    auto team= e->get("n/Team");
    auto gun= e->get("n/Gun");
    if (ENP(render) || ENP(team)) {
    continue; }

    auto enemy = closestOnTeam(e, OPPOSITE_TEAM(team->team));
    if (ENP(enemy)) {
    return; }

    auto enemyRender = (Render*)enemy->get("n/Render");
    if (ENP(enemyRender)) {
    continue; }

    auto distance = c::ccpDistance(render->pos(), enemyRender->pos());
    if (abs(distance) <= (gun->range + WIGGLE_ROOM) &&
        CACurrentMediaTime() - gun->lastDamageTime > gun->damageRate) {
      cx::sfxPlay(gun->sound);
      gun->lastDamageTime = CACurrentMediaTime();

      auto laser = engine->createLaser(team->team);
      auto laserRender = (Render*)laser->get("n/Render");
      auto laserMelee = (Melee*)laser->get("n/Melee");

      laserRender->setPos(render->pos());
      laserMelee->damage = gun->damage;

      auto direction = c::ccpNormalize(c::ccpSub(enemyRender->pos(), render->pos()));

      auto target = c::ccpMult(direction, laserDistance);
      auto duration = laserDistance / laserPointsPerSecond;

      laserRender->node->setRotation(-1 * CC_RADIANS_TO_DEGREES(ccpToAngle(direction)));
      laserRender->node->setZOrder(1);

      laserRender->node->runAction(
             c::Sequence::create(
              c::MoveBy::create(duration, target),
              c::RemoveSelf::create(true),
              CC_NIL));
    }
  }
}


NS_END

