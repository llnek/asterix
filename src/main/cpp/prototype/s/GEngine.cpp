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
  auto po= MGMS()->reifyPool("Asteroids");
  auto wb= cx::visBox();

  po->preset([=]() -> f::Poolable* {
    auto png = "Asteroid_" + FTOS(1 + (rand() % 3)) + ".png";
    auto tmp = f::CPixie::reifyFrame(png);
    auto sz= tmp->csize();
    CC_HIDE(tmp);
    MGML()->addItem(tmp, -1);
    auto body = c::PhysicsBody::createCircle(HWZ(sz));
    body->setContactTestBitmask(true);
    body->setDynamic(true);
    body->setEnabled(false);
    tmp->setPhysicsBody(body);
    auto ent= this->reifyNode("Asteroid");
    ent->checkin(tmp);
    return ent;
  }, 10);

  auto s= f::CPixie::reifyFrame("Space_Pod.png");
  auto ship= this->reifyNode("Ship", true);
  auto v= CC_CSV(c::Integer,"SHIP+SPEED");
  auto sz= s->csize();
  auto body = c::PhysicsBody::createCircle(HWZ(sz));
  body->setContactTestBitmask(true);
  body->setDynamic(true);
  s->setPhysicsBody(body);

  MGML()->addItem(s,-1);
  s->inflate(wb.cx, wb.cy);

  auto mo= mc_new(f::CGesture);
  auto mv= mc_new(f::CMove);
  mv->maxSpeed.y=v;
  mv->maxSpeed.x=v;
  mv->speed.y=v;
  mv->speed.x=v;
  ship->checkin(mo);
  ship->checkin(s);
  ship->checkin(mv);

  // global
  auto ent= this->reifyNode("Shared",true);
  auto ss= mc_new(GVars);
  ent->checkin(ss);

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

