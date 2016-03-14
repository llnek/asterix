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
NS_BEGIN(prototype)

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {
  auto p= MGMS()->reifyPool("Asteroids");
  auto wb= cx::visBox();

  p->preset([=]() -> f::Poolable* {
    auto png = "asteroid_" + s::to_string(1 + (rand() % 3)) + ".png";
    auto tmp = cx::reifySprite(png);
    auto sz= CC_CSIZE(tmp);
    CC_HIDE(tmp);
    MGML()->addItem(tmp,-1);
    auto body = c::PhysicsBody::createCircle(HWZ(sz));
    body->setContactTestBitmask(true);
    body->setDynamic(true);
    body->setEnabled(false);
    tmp->setPhysicsBody(body);
    auto rdr=mc_new1(f::CPixie, tmp);
    auto ent=this->reifyEntity();
    ent->checkin(rdr);
    return ent;
  }, 10);

  auto v= CC_CSV(c::Integer,"SHIP+SPEED");
  auto s= cx::reifySprite("player.png");
  auto sz= CC_CSIZE(s);
  auto body = c::PhysicsBody::createCircle(HWZ(sz));
  body->setContactTestBitmask(true);
  body->setDynamic(true);
  s->setPhysicsBody(body);

  auto rdr= mc_new1(f::CPixie,s);
  MGML()->addItem(s,-1);
  rdr->inflate(wb.cx, wb.cy);
  auto mo= mc_new(f::CGesture);
  auto mv= mc_new(f::CMove);
  mv->maxSpeed=v;
  mv->speed=v;
  auto ship= this->reifyEntity();
  ship->checkin(mc_new(f::CHuman));
  ship->checkin(rdr);
  ship->checkin(mo);
  ship->checkin(mv);

  // global
  this->reifyEntity()->checkin(mc_new(GVars));
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

