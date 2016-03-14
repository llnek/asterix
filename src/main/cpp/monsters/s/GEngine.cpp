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
#include "Resolve.h"
#include "AI.h"
#include "Move.h"
#include "Melee.h"
#include "Health.h"
#include "Player.h"
#include "Gun.h"
#include "n/AIStateMass.h"
#include "GEngine.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(monsters)

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {
  auto ent= this->reifyEntity();
  auto wb= cx::visBox();
  ent->checkin(mc_new(GVars));

  auto s= cx::reifySprite("castle1_def.png");
  s->setPosition(HWZ(CC_CSIZE(s)), wb.cy);
  MGML()->addAtlasItem("game-pics",s);
  // human
  ent= this->reifyEntity();
  ent->checkin(mc_new1(f::CHealth,200));
  ent->checkin(mc_new1(f::CPixie,s));
  ent->checkin(mc_new(f::CHuman));
  ent->checkin(mc_new1(Team,1));
  ent->checkin(mc_new(Player));
  ent->checkin(mc_new4(Gun,200,5,2,"pew"));

  s= cx::reifySprite("castle2_def.png");
  s->setPosition(wb.right - HWZ(CC_CSIZE(s)), wb.cy);
  MGML()->addAtlasItem("game-pics",s);
  // enemy
  ent= this->reifyEntity();
  ent->checkin(mc_new1(f::CHealth,200));
  ent->checkin(mc_new1(f::CPixie,s));
  ent->checkin(mc_new1(Team,2));
  ent->checkin(mc_new(Player));
  ent->checkin(mc_new4(Gun,200,5,2,"pew"));
  auto sm= mc_new(AIStateMass);
  ent->checkin(mc_new1(Automa,sm));

}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initSystems() {
  /*
  [_healthSystem update:dt];
    [_moveSystem update:dt];
    [_playerSystem update:dt];
    [_meleeSystem update:dt];
    [_gunSystem update:dt];
    [_aiSystem update:dt];
    */
  regoSystem(mc_new1(Resolve,this));
  regoSystem(mc_new1(GunLogic,this));
  regoSystem(mc_new1(AILogic,this));
  regoSystem(mc_new1(MoveLogic,this));
  regoSystem(mc_new1(MeleeLogic,this));
  regoSystem(mc_new1(HealthLogic,this));
  regoSystem(mc_new1(PlayerLogic,this));
}

//////////////////////////////////////////////////////////////////////////////
//
ecs::Entity* GEngine::createQuirkMonster(int team) {
  auto s= cx::reifySprite("quirk"+s::to_string(team)+".png");
  MGML()->addAtlasItem("game-pics",s);
  auto ent = this->reifyEntity();
  auto m= mc_new(Melee);
  m->damageRate=0.5;
  m->damage=1.25;
  m->sound="smallHit";
  ent->checkin(m);
  ent->checkin(mc_new3(f::CMove,c::Vec2(200, 200), 100, 100));
  ent->checkin(mc_new1(f::CPixie,s));
  ent->checkin(mc_new1(f::CHealth,5));
  ent->checkin(mc_new1(Team,team));
  ent->checkin(mc_new1(Monster, eMonsterTypeQuirk));

  return ent;
}

//////////////////////////////////////////////////////////////////////////////
//
ecs::Entity* GEngine::createZapMonster(int team) {
  auto s= cx::reifySprite("zap"+s::to_string(team)+".png");
  MGML()->addAtlasItem("game-pics",s);
  auto ent= this->reifyEntity();

  ent->checkin(mc_new3(f::CMove,c::Vec2(200, 200), 50, 50));
  ent->checkin(mc_new1(f::CHealth,10));
  ent->checkin(mc_new1(f::CPixie,s));
  ent->checkin(mc_new1(Team,team));
  ent->checkin(mc_new4(Gun,100, 5, 1.5,"pew"));
  ent->checkin(mc_new1(Monster, eMonsterTypeZap));

  return ent;
}

//////////////////////////////////////////////////////////////////////////////
//
ecs::Entity* GEngine::createMunchMonster(int team) {
  auto s= cx::reifySprite("munch"+s::to_string(team)+".png");
  MGML()->addAtlasItem("game-pics",s);
  auto ent= this->reifyEntity();
  auto m= mc_new(Melee);
  m->damage=10;
  m->damageRate=2;
  m->aoe=true;
  m->sound="bigHit";
  ent->checkin(m);
  ent->checkin(mc_new1(f::CPixie,s));
  ent->checkin(mc_new1(f::CHealth,50));
  ent->checkin(mc_new3(f::CMove,c::Vec2(200, 200), 25, 25));
  ent->checkin(mc_new1(Team,team));
  ent->checkin(mc_new1(Monster,eMonsterTypeMunch));

  return ent;
}

//////////////////////////////////////////////////////////////////////////////
//
ecs::Entity* GEngine::createLaser(int team) {
  auto s= cx::reifySprite("laser"+s::to_string(team)+".png");
  MGML()->addAtlasItem("game-pics",s);
  auto ent= this->reifyEntity();
  auto m= mc_new(Melee);
  m->damageRate=1;
  m->damage=5;
  m->selfDie=true;
  m->sound="smallHit";
  ent->checkin(m);
  ent->checkin(mc_new1(f::CPixie,s));
  ent->checkin(mc_new1(Team,team));

  return ent;
}



NS_END

