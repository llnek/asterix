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

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "GEngine.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(asteroids)

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {

}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initSystems() {

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
    return mc_new_1(Laser, sp);
  }, count);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createShip() {
  auto sp= cx::reifySprite("rship_0.png");
  auto ent= this->reifyEntity("*");
  auto s= mc_new_1(Ship, sp)
  MGML()->addAtlasItem("game-pics", sp);
  sp->setRotation(90);
  s->maxVel.x=150;
  s->maxVel.y=150;
  s->power=25;
  s->degree=90;
  ent->checkin(s);
  bornShip();
  ent->checkin(mc_new(Motion));
  ent->checkin(mc_new(Cannon));
  ent->checkin(mc_new_1(Looper,1));
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
  auto sz= cx::calcSize("astro1");
  auto cfg = MGMS()->getLCfg();
  auto ht = sz.height;
  auto wd = sz.width;
  auto pn="";

  switch (rank) {
    case 1: pn = "Astros1"; break;
    case 2: pn = "Astros2"; break;
    case 3: pn = "Astros3"; break;
    default: return;
  }

  auto pool= MGMS()->getPool(pn);
  auto wz = cx::visRect();
  auto cw= cx::center();

  auto obj= JS_OBJ(cfg[pn]);
  int cnt= JS_INT(obj["num"]);
  int n=0;
  float x,y,deg;
  f::Box4 r(0,0,0,0);

  while (n < cnt) {
    r.right= cx.randFloat(wz.size.width);
    r.top= cx::randFloat(wz.size.height);
    r.bottom = r.top - ht;
    r.left = r.right - wd;
    if ( !cx::outOfBound(r,B)) {
      pool->preset([=]() -> f::ComObj* {
        deg = cx::randFloat(360.0f);
        x = r.left + wd * 0.5f;
        y = r.top - ht * 0.5f;
        auto sp= cx::reifySprite(JS_STR(obj["img"]));
        auto v= JS_INT(obj["speed"]);
        sp->setRotation(deg);
        MGML()->addAtlasItem("game-pics", sp);
        auto astro= new Asteroid(sp, JS_INT(obj["value"]), rank, deg,
                               v * cx::randSign(),
                               v * cx::randSign());
        astro->inflate(x, y);
        return astro;
      },1);
      ++n;
    }
  }

  CCLOG("CREATED more asteroids - %d", rank);
}

//////////////////////////////////////////////////////////////////////////////
//
bool GEngine::maybeOverlap(Ship *ship) {
  auto p= MGMS()->getPool("Astros1");
  auto bx= cx::bbox4(ship);
  auto rc=false;

  rc= p->some([=](f::ComObj* z) -> bool {
      return z->status ? cx::isIntersect(bx, cx::bbox4(z)) : false;
  });
  if (rc) { return true; }

  p= MGMS()->getPool("Astros2");
  rc= p->some([=](f::ComObj* z) -> bool {
      return z->status ? cx::isIntersect(bx, cx::bbox4(z)) : false;
  });
  if (rc) { return true; }

  p= MGMS()->getPool("Astros3");
  rc= p->some([=](f::ComObj* z) -> bool {
      return z->status ? cx::isIntersect(bx, cx::bbox4(z)) : false;
  });
  if (rc) { return true; }

  return false;
}

NS_END(asteroids)



