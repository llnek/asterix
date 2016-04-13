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
  auto ents = _engine->getNodes(
      s_vec<ecs::COMType>{"n/Team", "n/Gun", "f/CPixie"});
  F__LOOP(it,ents) {
    auto &e= *it;
    auto render = CC_GEC(f::CPixie,e,"f/CPixie");
    auto team = CC_GEC(Team,e,"n/Team");
    auto gun = CC_GEC(Gun,e,"n/Gun");
    auto other = closestEntOnTeam(_engine,e, OTHER_TEAM(team->team));
    if (!other) { return; }

    auto otherRender = CC_GEC(f::CPixie,other,"f/CPixie");
    auto dist = cx::calcDist(render, otherRender);

    if (abs(dist) <= (gun->range + WIGGLE_ROOM) &&
        (cx::timeInMillis() - gun->lastDamageTime) > gun->damageRate) {

      gun->lastDamageTime = cx::timeInMillis();
      cx::sfxPlay(gun->sound);

      auto laser = SCAST(GEngine*,_engine)->createLaser(team->team);
      auto laserRender = CC_GEC(f::CPixie,laser,"f/CPixie");
      auto laserMelee = CC_GEC(Melee,laser,"f/CMelee");

      CC_POS1(laserRender, render->pos());
      laserMelee->damage = gun->damage;

      auto duration = laserDistance / laserPointsPerSecond;
      auto dir= cx::normalize(render,otherRender);
      auto target = c::ccpMult(dir, laserDistance);

      laserRender->setRotation( - CC_RAD_TO_DEG(c::ccpToAngle(dir)));
      laserRender->setZOrder(1);

      laserRender->runAction(
         c::Sequence::create(
          c::MoveBy::create(duration, target),
          c::CallFunc::create([=](){
            _engine->purgeNode(laser);
            }),
          c::RemoveSelf::create(),
          CC_NIL));
    }
  }
}



NS_END

