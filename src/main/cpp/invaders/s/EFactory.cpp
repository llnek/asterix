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
#include "EFactory.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
EFactory::EFactory(not_null<a::Engine*> e,
    not_null<c::Dictionary*> options)

  : f::Factory(e,options) {

}

//////////////////////////////////////////////////////////////////////////
//
EFactory::~EFactory() {
}

//////////////////////////////////////////////////////////////////////////
//
void EFactory::reifyMissiles(int count) {
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
void EFactory::reifyExplosions(int count) {
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
void EFactory::reifyBombs(int count) {
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
const c::Size EFactory::calcImgSize(const stdstr& img) {
  return cx::calcSize(img);
}

//////////////////////////////////////////////////////////////////////////
//
const c::Size EFactory::getRankInfo(int r, c::Dictionary* out) {
  c::Size z= cx::calcSize("purple_bug_0.png");
  stdstr s0 = "purple_bug_0.png";
  stdstr s1= "purple_bug_1.png";
  int v= 30;

  if (r < 3) {
    v= 100;
    s0 = "blue_bug_0.png";
    s1 = "blue_bug_1.png";
    z= calcImgSize("blue_bug_0.png");
  }
  else
  if (r < 5) {
    v= 50;
    s0 = "green_bug_0.png";
    s1= "green_bug_1.png";
    z = calcImgSize("green_bug_0.png");
  }

  out->removeAllObjects();
  out->setObject(f::Size2::create(z.width, z.height), "size");
  out->setObject(CC_INT(v), "value");
  out->setObject(CC_STR(s0), "img0");
  out->setObject(CC_STR(s1), "img1");

  return CC_GDV(f::Size2, out, "size");
}

//////////////////////////////////////////////////////////////////////////
//
void EFactory::fillSquad(f::XPool* pool) {

  auto cache= c::AnimationCache::getInstance();
  auto rows = CC_CSV(c::Integer, "ROWS");
  auto cols = CC_CSV(c::Integer, "COLS");
  auto wz= cx::visRect();
  auto wb= cx::visBox();
  auto info= CC_DICT();
  float x;
  float y;

  for (int r=0; r < rows; ++r) {
    auto az = getRankInfo(r, info);
    y = (r == 0) ? wb.top * 0.9f
                   : y - az.height - wz.size.height * 4/480.0f;
    x = wb.left + (8/320.0f * wz.size.width) + HWZ(az);
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
      x += az.width + (8/320.0f * wz.size.width);
      auto v = CC_GDV(c::Integer, info, "value");
      auto co= mc_new_3(Alien, aa, v, r);
      co->status=true;
      pool->checkin(co);
    }
  }
/*
  for (int n=0; n < cells; ++n) {

    auto az= CC_GDV(f::Size2, info, "size");
    if (n % cols == 0) {
      y = (n == 0) ? wb.top * 0.9f
                   : y - az.height - wz.size.height * 4/480.0f;
      x = wb.left + (8/320.0f * wz.size.width) + HWZ(az);
      row += 1;
      info= getRankInfo(row);
      az= CC_GDV(f::Size2, info, "size");
    }

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
    x += az.width + (8/320.0f * wz.size.width);
    auto v = CC_GDV(c::Integer, info, "value");
    auto co= mc_new_3(Alien, aa, v, row);
    co->status=true;
    pool->checkin(co);
  }
*/
}

//////////////////////////////////////////////////////////////////////////
//
a::Entity* EFactory::reifyAliens() {
  auto stepx= CC_GDV(f::Size2, state, "alienSize").width / 3.0f;
  auto ent= engine->reifyEntity("baddies");
  auto p = MGMS()->getPool("aliens");

  fillSquad(p);

  ent->checkin(mc_new_2(AlienSquad, p, stepx));
  ent->checkin(mc_new(Looper));

  return ent;
}

//////////////////////////////////////////////////////////////////////////
//
void EFactory::bornShip() {
  CCASSERT(player != nullptr, "player cannot be null");
  player->inflate();
}

//////////////////////////////////////////////////////////////////////////
//
a::Entity* EFactory::reifyShip() {

  auto sz = CC_GDV(f::Size2, state, "shipSize");
  auto ent= engine->reifyEntity("goodies");
  auto s= cx::reifySprite("ship_1.png");
  auto wz= cx::visRect();
  auto wb= cx::visBox();
  auto y = sz.height + wb.bottom + (5/60.0f * wz.size.height);
  auto x = wb.left + wz.size.width * 0.5f;
  auto ship = mc_new_3(Ship, s, "ship_1.png", "ship_0.png");

  CCASSERT(s != nullptr, "ship sprite cannot be null");

  MGML()->addAtlasItem("game-pics", s);
  ship->inflate(x,y);
  player= ship;

  ent->checkin(mc_new_2(Velocity, 150,0));
  ent->checkin(mc_new(Looper));
  ent->checkin(mc_new_1(Cannon,1.0f));
  ent->checkin(mc_new(Motion));

  ent->checkin(ship);
  return ent;
}



NS_END(invaders)


