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

#include "x2d/GameLayer.h"
#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "core/XPool.h"
#include "ash/Engine.h"
#include "n/GNodes.h"
#include "Stage.h"
#include "Motion.h"
#include "Move.h"
#include "Collide.h"
#include "Resolve.h"
#include "Aliens.h"
#include "EFactory.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::ignite() {
  regoSystem(mc_new( Stage));
  regoSystem(mc_new( Motions));
  regoSystem(mc_new( Move));
  regoSystem(mc_new(Aliens));
  regoSystem(mc_new( Collide));
  regoSystem(mc_new( Resolve));
}

//////////////////////////////////////////////////////////////////////////////
//
a::Entity* GEngine::reifyArena() {
  //pick purple since it is the largest
  auto z0= cx::calcSize("purple_bug_0.png");
  auto s0= cx::calcSize("ship_0.png");
  auto ent= reifyEntity("*");
  auto slots = mc_new(Slots);

  slots->alienSize = z0;
  slots->shipSize = s0;
  ent->checkin(slots);

  return ent;
}

//////////////////////////////////////////////////////////////////////////
//
void GEngine::reifyMissiles(int count) {
  auto p= MGMS()->getPool("missiles");
  p->preset([=]() -> f::ComObj* {
    auto sp = cx::reifySprite("missile.png");
    sp->setVisible(false);
    MGML()->addAtlasItem("game-pics", sp);
    return mc_new_1(Missile, sp);
  }, count);
}

//////////////////////////////////////////////////////////////////////////
//
void GEngine::reifyExplosions(int count) {
  auto p= MGMS()->getPool("explosions");
  p->preset([=]() -> f::ComObj* {
    auto sp = cx::reifySprite("boom_0.png");
    sp->setVisible(false);
    MGML()->addAtlasItem("game-pics", sp);
    return mc_new_1(Explosion, sp);
  }, count);
}

//////////////////////////////////////////////////////////////////////////
//
void GEngine::reifyBombs(int count) {
  auto p= MGMS()->getPool("bombs");
  p->preset([=]() -> f::ComObj* {
    auto sp = cx::reifySprite("bomb.png");
    sp->setVisible(false);
    MGML()->addAtlasItem("game-pics", sp);
    return mc_new_1(Bomb, sp);
  }, count);
}

//////////////////////////////////////////////////////////////////////////
//
const c::Size GEngine::getRankInfo(int r, c::Dictionary *out) {
  c::Size z= cx::calcSize("purple_bug_0.png");
  sstr s0 = "purple_bug_0.png";
  sstr s1= "purple_bug_1.png";
  int v= 30;

  if (r < 3) {
    v= 100;
    s0 = "blue_bug_0.png";
    s1 = "blue_bug_1.png";
    z= cx::calcSize("blue_bug_0.png");
  }
  else
  if (r < 5) {
    v= 50;
    s0 = "green_bug_0.png";
    s1= "green_bug_1.png";
    z = cx::calcSize("green_bug_0.png");
  }

  out->setObject(f::Size2::create(z), "size");
  out->setObject(CC_INT(v), "value");
  out->setObject(CC_STR(s0), "img0");
  out->setObject(CC_STR(s1), "img1");

  return z;
}

//////////////////////////////////////////////////////////////////////////
//
void GEngine::fillSquad(f::XPool *pool) {

  auto cache= c::AnimationCache::getInstance();
  auto rows = CC_CSV(c::Integer, "ROWS");
  auto cols = CC_CSV(c::Integer, "COLS");
  auto gz= XCFG()->gameSize();
  auto wz= cx::visRect();
  auto wb= cx::visBox();
  auto info= CC_DICT();
  float x;
  float y;

  for (int r=0; r < rows; ++r) {
    auto az = getRankInfo(r, info);
    y = (r == 0) ? wb.top * 0.9f
         : y - az.height - wz.size.height * 4/gz.height;
    x = wb.left + (8/gz.width * wz.size.width) + HWZ(az);
    for (int c=0; c < cols; ++c) {
      auto s = CC_GDS(info, "img0");
      auto aa = cx::reifySprite(s);
      aa->setPosition(x + HWZ(az), y - HHZ(az));

      auto f1= CC_GDS(info, "img0");
      auto f2= CC_GDS(info, "img1");
      c::Vector<c::SpriteFrame*> animFrames(2);

      animFrames.pushBack( cx::getSpriteFrame(f1));
      animFrames.pushBack( cx::getSpriteFrame(f2));

      aa->runAction(c::RepeatForever::create(
        c::Animate::create(
          c::Animation::createWithSpriteFrames( animFrames, 1))));

      MGML()->addAtlasItem("game-pics", aa);
      x += az.width + (8/gz.width * wz.size.width);
      auto v = CC_GDV(c::Integer, info, "value");
      auto co= mc_new_3(Alien, aa, v, r);
      co->status=true;
      pool->checkin(co);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
a::Entity* GEngine::reifyAliens() {
  // use the largest one to calc size
  auto z0= cx::calcSize("purple_bug_0.png");
  auto stepx= z0.width / 3.0f;
  auto ent= this->reifyEntity("baddies");
  auto p = MGMS()->getPool("aliens");

  fillSquad(p);

  ent->checkin(mc_new_2(AlienSquad, p, stepx));
  ent->checkin(mc_new(Looper));
  return ent;
}

//////////////////////////////////////////////////////////////////////////
//
a::Entity* GEngine::reifyShip() {

  auto ent= this->reifyEntity("goodies");
  auto sz= cx::calcSize("ship_0.png");
  auto s= cx::reifySprite("ship_1.png");
  auto wz= cx::visRect();
  auto wb= cx::visBox();
  //TODO: why 60
  auto y = sz.height + wb.bottom + (5/60.0f * wz.size.height);
  auto x = wb.left + wz.size.width * 0.5f;
  auto ship = mc_new_3(Ship, s, "ship_1.png", "ship_0.png");

  CCASSERT(s != nullptr, "ship sprite cannot be null");

  MGML()->addAtlasItem("game-pics", s);
  ship->inflate(x,y);
  ship->vel.x= 150;
  ship->vel.y= 150;
  //player= ship;

  ent->checkin(mc_new(Looper));
  ent->checkin(mc_new(Cannon));
  ent->checkin(mc_new(Motion));

  ent->checkin(ship);
  return ent;
}



NS_END(invaders)


