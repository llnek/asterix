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
#include "n/Enemy.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(bazuka)

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {

  auto po= MGMS()->reifyPool("Rockets");
  po->preset([=]() -> f::Poolable* {
    auto sp= cx::reifySprite("rocket.png");
    CC_HIDE(sp);
    MGML()->addAtlasItem("game-pics", sp);
    return mc_new1(Projectile,sp);
  }, 32);

  po= MGMS()->reifyPool("Bullets");
  po->preset([=]() -> f::Poolable* {
    auto sp= cx::reifySprite("bullet.png");
    CC_HIDE(sp);
    MGML()->addAtlasItem("game-pics", sp);
    return mc_new1(Projectile,sp);
  }, 32);

  po= MGMS()->reifyPool("Enemies");
  po->preset([=]() -> f::Poolable* {
    return Enemy::create();
  }, 32);

  auto sp = cx::reifySprite("player_idle_1.png");
  auto ent= this->reifyEntity();
  auto py= mc_new1(Hero,sp);
  auto wb= cx::visBox();

  ent->checkin(mc_new(Gesture));
  ent->checkin(py);

  ent=this->reifyEntity();
  ent->checkin(mc_new(GVars));
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initSystems() {
  regoSystem(mc_new1(Resolve,this));
  regoSystem(mc_new1(Collide,this));
  regoSystem(mc_new1(AI,this));
  regoSystem(mc_new1(Move,this));
}


NS_END

