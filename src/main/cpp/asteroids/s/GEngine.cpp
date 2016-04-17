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
#include "Motion.h"
#include "Move.h"
#include "Collide.h"
#include "GEngine.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(asteroids)

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {
  MGMS()->reifyPools(s_vec<sstr> {
    "Missiles",
    "Lasers",
    "Astros3",
    "Astros2",
    "Astros1"
  });

  astroSizes[3]= XCFG()->fit(cx::calcSize("rock_small.png"));
  astroSizes[2]= XCFG()->fit(cx::calcSize("rock_med.png"));
  astroSizes[1]= XCFG()->fit(cx::calcSize("rock_large.png"));

  astroPools[3]= "Astros3";
  astroPools[2]= "Astros2";
  astroPools[1]= "Astros1";

  auto ent= this->reifyNode("Arena", true);
  auto ss= mc_new(GVars);
  ent->checkin(ss);

  createAsteroids(1);
  createShip();

  createMissiles();
  createLasers();
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initSystems() {
  regoSystem(mc_new1(Resolve,this));
  regoSystem(mc_new1(Move,this));
  regoSystem(mc_new1(Motions,this));
  regoSystem(mc_new1(Collide,this));
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createMissiles(int count) {
  auto po= MGMS()->getPool("Missiles");
  po->preset([=]() -> f::Poolable* {
    auto sp = f::CPixie::reifyFrame("laserGreen.png");
    CC_HIDE(sp);
    auto e= this->reifyNode("Missile");
    auto mv= mc_new(f::CMove);
    mv->speed.x=20;
    mv->speed.y=20;
    MGML()->addAtlasItem("game-pics", sp);
    e->checkin(mc_new(f::CHealth));
    e->checkin(mv);
    e->checkin(sp);
    return e;
  }, count);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createLasers(int count) {
  auto po= MGMS()->getPool("Lasers");
  po->preset([=]() -> f::Poolable* {
    auto sp = f::CPixie::reifyFrame("laserRed.png");
    CC_HIDE(sp);
    auto e= this->reifyNode("Laser");
    auto mv= mc_new(f::CMove);
    mv->speed.x=20;
    mv->speed.y=20;
    MGML()->addAtlasItem("game-pics", sp);
    e->checkin(mc_new(f::CHealth));
    e->checkin(mv);
    e->checkin(sp);
    return e;
  }, count);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createShip() {
  auto ent= this->reifyNode("Ship", true);
  auto sp= Ship::create();
  auto mv= mc_new(f::CMove);
  MGML()->addAtlasItem("game-pics", sp);
  sp->setRotation(90);
  mv->maxSpeed.x= 150;
  mv->maxSpeed.y= 150;
  mv->power=25;
  mv->angle=90;
  ent->checkin(mv);
  ent->checkin(sp);
  ent->checkin(mc_new(f::CGesture));
  ent->checkin(mc_new(f::CHealth));
  ent->checkin(mc_new(ShipStats));
  ent->checkin(mc_new(f::Looper));
  bornShip(ent);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::bornShip(not_null<ecs::Node*> node) {
  auto ship= CC_GEC(Ship,node,"f/CPixie");
  auto B= MGMS()->getEnclosureBox();
  auto sz = ship->csize();
  auto wz = cx::visSize();
  auto pos= ship->pos();
  auto h= sz.height;
  auto w = sz.width;
  auto test=true;
  auto x= pos.x;
  auto y= pos.y;
  f::Box4 r;

  while (test) {

    r.right= cx::randFloat(wz.width);
    r.top= cx::randFloat(wz.height);
    r.bottom = r.top - h ;
    r.left= r.right - w ;

    if (!maybeOverlap(r) &&
        !cx::outOfBound(r,B)) {
      x = r.left + HTV(w);
      y = r.top - HTV(h);
      test=false;
    }
  }

  ship->inflate(x, y);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createAsteroids(int rank) {
  CCLOG("about to create more asteroids - %d", rank);
  auto B= MGMS()->getEnclosureBox();
  assert(rank > 0 && rank < 4);
  auto cfg = MGMS()->getLCfg()->getValue();
  auto sz= astroSizes[rank];
  auto pn= astroPools[rank];
  auto wz = cx::visSize();

  auto pool= MGMS()->getPool(pn);
  auto obj= JS_OBJ(cfg[pn]);
  auto value= JS_INT(obj["value"]);
  auto speed= JS_INT(obj["speed"]);
  int cnt= JS_INT(obj["num"]);
  auto img= JS_STR(obj["img"]);
  int n=0;
  f::Box4 r;

  while (n < cnt) {
    r.right= cx::randFloat(wz.width);
    r.top= cx::randFloat(wz.height);
    r.bottom = r.top - sz.height;
    r.left = r.right - sz.width;
    if ( !cx::outOfBound(r,B)) {
      pool->preset([=]() -> f::Poolable* {
        auto ent=this->reifyNode("Asteroid", true);
        auto deg = cx::randFloat(360);
        auto x = r.left + HWZ(sz);
        auto y = r.top - HHZ(sz);
        auto sp= Asteroid::create(img, rank);
        sp->setRotation(deg);
        MGML()->addAtlasItem("game-pics", sp);
        auto a= mc_new1(f::CStats,value);
        auto mv=mc_new(f::CMove);
        mv->vel.y= speed * cx::randSign();
        mv->vel.x= speed * cx::randSign();
        mv->speed.x= speed;
        mv->speed.y= speed;
        mv->angle=deg;
        ent->checkin(mc_new(f::CHealth));
        ent->checkin(sp);
        ent->checkin(mv);
        ent->checkin(a);
        sp->inflate(x, y);
        return ent;
      }, 1);
      ++n;
    }
  }

  CCLOG("CREATED %d more asteroids{%d}", cnt, rank);
}

//////////////////////////////////////////////////////////////////////////////
//
bool GEngine::maybeOverlap(const f::Box4 &bx) {
  auto p= MGMS()->getPool(astroPools[1]);
  auto rc=false;

  rc= p->some([=](f::Poolable *z) -> bool {
      auto e= (ecs::Node*)z;
      auto s= CC_GEC(f::CPixie,e,"f/CPixie");
      return e->status() ? cx::isIntersect(bx, cx::bbox4(s)) : false;
  });
  if (rc) { return true; }

  p= MGMS()->getPool(astroPools[2]);
  rc= p->some([=](f::Poolable *z) -> bool {
      auto e= (ecs::Node*)z;
      auto s= CC_GEC(f::CPixie,e,"f/CPixie");
      return z->status() ? cx::isIntersect(bx, cx::bbox4(s)) : false;
  });
  if (rc) { return true; }

  p= MGMS()->getPool(astroPools[3]);
  rc= p->some([=](f::Poolable *z) -> bool {
      auto e= (ecs::Node*)z;
      auto s= CC_GEC(f::CPixie,e,"f/CPixie");
      return z->status() ? cx::isIntersect(bx, cx::bbox4(s)) : false;
  });
  if (rc) { return true; }

  return false;
}


NS_END



