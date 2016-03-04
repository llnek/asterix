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
#include "Collide.h"
#include "Move.h"
#include "AI.h"
#include "GEngine.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(monsters)

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {
  auto ent= this->reifyEntity();
  auto wb= cx::visBox();
  ent->checkin(mc_new(GVars));

  // human
  ent= this->reifyEntity();
  ent->checkin(mc_new(Render));
  ent->checkin(mc_new(Health));
  ent->checkin(mc_new(Team));
  ent->checkin(mc_new(Player));
  ent->checkin(mc_new(Gun));

  // enemy
  ent= this->reifyEntity();
  ent->checkin(mc_new(Render));
  ent->checkin(mc_new(Health));
  ent->checkin(mc_new(Team));
  ent->checkin(mc_new(Player));
  ent->checkin(mc_new(Gun));
  ent->checkin(mc_new(Automa));

}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initSystems() {
  regoSystem(mc_new1(Resolve,this));
  regoSystem(mc_new1(Collide,this));
  regoSystem(mc_new1(AI,this));
  regoSystem(mc_new1(Move,this));
}

//////////////////////////////////////////////////////////////////////////////
//
Entity* GEngine::createQuirkMonster(int team) {
  auto s= cx::reifySprite("quirk"+s::to_string(team)+".png");
  MGML()->addAtlasItem("game-pics",s);
  auto ent = this->reifyEntity();

  ent->checkin(new Melee(1.25,false,0.5,false,"smallHit"));
  ent->checkin(new Move(c::Vec2(200, 200), 100, 100));
  ent->checkin(mc_new1(Render,s));
  ent->checkin(mc_new2(Health,5,5));
  ent->checkin(mc_new1(Team,team));
  ent->checkin(mc_new1(Monster, eMonsterTypeQuirk));

  return ent;
}

//////////////////////////////////////////////////////////////////////////////
//
Entity* GEngine::createZapMonster(int team) {
  auto s= cx::reifySprite("zap"+s::to_string(team)+".png");
  MGML()->addAtlasItem("game-pics",s);
  auto ent= this->reifyEntity();

  ent->checkin(new Render(s));
  ent->checkin(new Health(10, 10));
  ent->checkin(new Move(c::Vec2(200, 200), 50, 50));
  ent->checkin(new Team(team));
  ent->checkin(new Gun(100, 5, 1.5,"pew"));
  ent->checkin(new Monster(eMonsterTypeZap));

  return ent;
}

//////////////////////////////////////////////////////////////////////////////
//
Entity* GEngine::createMunchMonster(int team) {
  auto s= cx::reifySprite("munch"+s::to_string(team)+".png");
  MGML()->addAtlasItem("game-pics",s);
  auto ent= this->reifyEntity();

  ent->checkin(new Render(s));
  ent->checkin(new Health(50, 50));
  ent->checkin(new Move(c::Vec2(200, 200), 25, 25));
  ent->checkin(new Team(team));
  ent->checkin(new Melee(10.0, false, 2.0, true, "bigHit"));
  ent->checkin(new Monster(eMonsterTypeMunch));

  return ent;
}

//////////////////////////////////////////////////////////////////////////////
//
Entity* GEngine::createLaser(int team) {
  auto s= cx::reifySprite("laser"+s::to_string(team)+".png");
  MGML()->addAtlasItem("game-pics",s);
  auto ent= this->reifyEntity();

  ent->checkin(new Render(s));
  ent->checkin(new Team(team));
  ent->checkin(new Melee(5, true, 1.0, false, "smallHit"));

  return ent;
}



NS_END

