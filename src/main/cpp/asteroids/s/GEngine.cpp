// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2015, Ken Leung. All rights reserved.

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

  astroSizes[3]= cx::calcSize("rock_small.png");
  astroSizes[2]= cx::calcSize("rock_med.png");
  astroSizes[1]= cx::calcSize("rock_large.png");

  astroPools[3]= "Astros3";
  astroPools[2]= "Astros2";
  astroPools[1]= "Astros1";

  createAsteroids(1);
  createShip();
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initSystems() {
  MGMS()->reifyPools(s_vec<sstr> {
        "Missiles",
        "Lasers",
        "Astros3",
        "Astros2",
        "Astros1"
      });
  regoSystem(mc_new_1(Resolve,this));
  regoSystem(mc_new_1(Move,this));
  regoSystem(mc_new_1(Motions,this));
  regoSystem(mc_new_1(Collide,this));
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createMissiles(int count) {
  auto p= MGMS()->getPool("Missiles");
  p->preset([=]() -> f::ComObj* {
    auto sp = cx::reifySprite("laserGreen.png");
    sp->setVisible(false);
    MGML()->addAtlasItem("game-pics", sp);
    return mc_new_1(Missile, sp);
  }, count);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createLasers(int count) {
  auto p= MGMS()->getPool("Lasers");
  p->preset([=]() -> f::ComObj* {
    auto sp = cx::reifySprite("laserRed.png");
    sp->setVisible(false);
    MGML()->addAtlasItem("game-pics", sp);
    return mc_new_1(Missile, sp);
  }, count);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createShip() {
  auto sp= cx::reifySprite("rship_0.png");
  auto ent= this->reifyEntity("*");
    auto s= mc_new_1(Ship, sp);
  MGML()->addAtlasItem("game-pics", sp);
  sp->setRotation(90);
  s->maxVel.x=150;
  s->maxVel.y=150;
  s->power=25;
  s->angle=90;
  ent->checkin(s);
  bornShip(s);
  ent->checkin(mc_new(Motion));
  ent->checkin(mc_new(Cannon));
  ent->checkin(mc_new(Looper));
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::bornShip(Ship *ship) {
  auto sz = ship->sprite->getContentSize();
  auto B= MGMS()->getEnclosureBox();
  auto h= sz.height;
  auto w = sz.width;
  auto wz = cx::visRect();
  auto cw = cx::center();
  auto test=true;
    auto pos= ship->pos();
    auto x= pos.x;
    auto y= pos.y;
  f::Box4 r(0,0,0,0);

  while (test) {

    r.right= cx::randFloat(wz.size.width);
    r.top= cx::randFloat(wz.size.height);
    r.bottom = r.top - h ;
    r.left= r.right - w ;

    if (!maybeOverlap(r) &&
        !cx::outOfBound(r,B)) {
      x = r.left + w * 0.5f;
      y = r.top - h * 0.5f;
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
  auto wz = cx::visRect();
  auto cw= cx::center();

  auto pool= MGMS()->getPool(pn);
  auto obj= JS_OBJ(cfg[pn]);
  auto value= JS_INT(obj["value"]);
  auto speed= JS_INT(obj["speed"]);
  auto cnt= JS_INT(obj["num"]);
  auto img= JS_STR(obj["img"]);
  int n=0;
  //float x,y;
  f::Box4 r(0,0,0,0);

  while (n < cnt) {
    r.right= cx::randFloat(wz.size.width);
    r.top= cx::randFloat(wz.size.height);
    r.bottom = r.top - sz.height;
    r.left = r.right - sz.width;
    if ( !cx::outOfBound(r,B)) {
      pool->preset([=]() -> f::ComObj* {
        auto deg = cx::randFloat(360.0f);
        auto x = r.left + sz.width * 0.5f;
        auto y = r.top - sz.height * 0.5f;
        auto sp= cx::reifySprite(img);
        sp->setRotation(deg);
        MGML()->addAtlasItem("game-pics", sp);
        auto a= new Asteroid(sp, value, rank, deg,
                             c::Vec2(
                             speed * cx::randSign(),
                               speed * cx::randSign()));
        a->inflate(x, y);
        return a;
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

  rc= p->some([=](f::ComObj* z) -> bool {
      return z->status ? cx::isIntersect(bx, cx::bbox4(z)) : false;
  });
  if (rc) { return true; }

  p= MGMS()->getPool(astroPools[2]);
  rc= p->some([=](f::ComObj* z) -> bool {
      return z->status ? cx::isIntersect(bx, cx::bbox4(z)) : false;
  });
  if (rc) { return true; }

  p= MGMS()->getPool(astroPools[3]);
  rc= p->some([=](f::ComObj* z) -> bool {
      return z->status ? cx::isIntersect(bx, cx::bbox4(z)) : false;
  });
  if (rc) { return true; }

  return false;
}

NS_END(asteroids)



