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

#include "x2d/GameLayer.h"
#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Motion.h"
#include "Move.h"
#include "Collide.h"
#include "Resolve.h"
#include "Aliens.h"
#include "GEngine.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {

  MGMS()->reifyPools(
      s_vec<sstr> {"Explosions", "Aliens",
                   "Missiles", "Bombs"});

  auto ss=reifyArena();
  reifyAliens(ss);
  reifyShip(ss);

  reifyExplosions();
  reifyMissiles();
  reifyBombs();
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initSystems() {
  regoSystem(mc_new1(Motions,this));
  regoSystem(mc_new1(Move,this));
  regoSystem(mc_new1(Aliens,this));
  regoSystem(mc_new1(Collide,this));
  regoSystem(mc_new1(Resolve,this));
}

//////////////////////////////////////////////////////////////////////////////
//
GVars* GEngine::reifyArena() {
  auto ent= this->reifyNode("Arena", true);
  auto ss = mc_new(GVars);

  ss->purple = cx::calcSize("purple_bug_0.png");
  ss->blue = cx::calcSize("blue_bug_0.png");
  ss->green = cx::calcSize("green_bug_0.png");
  ss->shipSize = cx::calcSize("ship_0.png");

  ent->checkin(ss);
  return ss;
}

//////////////////////////////////////////////////////////////////////////
//
ecs::Node* GEngine::reifyAliens(GVars *ss) {
  //pick purple since it is the largest
  //use the largest one to calc size
  auto ent= this->reifyNode("AlienSquad", true);
  auto stepx= ss->purple.width / 3.0;
  auto p = MGMS()->getPool("Aliens");

  fillSquad(ss,p);

  ent->checkin(mc_new2(AlienSquad, p, stepx));
  ent->checkin(mc_new(f::Loopers));
  return ent;
}

//////////////////////////////////////////////////////////////////////////
//
ecs::Node* GEngine::reifyShip(GVars *ss) {

  auto ent= this->reifyNode("Ship", true);
  auto s= Ship::create();
  auto sz= CC_CSIZE(s);
  auto wz= cx::visSize();
  auto wb= cx::visBox();

  auto y = 2 * sz.height + wb.bottom;
  auto mv= mc_new(f::CMove);

  MGML()->addAtlasItem("game-pics", s);
  s->inflate(wb.cx,y);
  mv->speed.x =150;

  ent->checkin(mc_new(f::CGesture));
  ent->checkin(mc_new(f::CHealth));
  ent->checkin(mc_new(ShipStats));
  ent->checkin(mc_new(f::Looper));
  ent->checkin(s);
  ent->checkin(mv);

  return ent;
}

//////////////////////////////////////////////////////////////////////////
//
void GEngine::reifyMissiles(int count) {
  auto p= MGMS()->getPool("Missiles");
  auto gz= XCFG()->gameSize();
  auto wz= cx::visSize();

  p->preset([=]() -> f::Poolable* {
    auto sp = f::CPixie::reifyFrame("missile.png");
    auto mv= mc_new(f::CMove);
    CC_HIDE(sp);
    mv->speed.y = 150.0 * wz.height / gz.height;
    MGML()->addAtlasItem("game-pics", sp);
    auto ent= this->reifyNode("Missile");
    ent->checkin(mc_new(f::CHealth));
    ent->checkin(sp);
    ent->checkin(mv);
    return ent;
  }, count);

}

//////////////////////////////////////////////////////////////////////////
//
void GEngine::reifyExplosions(int count) {
  auto p= MGMS()->getPool("Explosions");
  p->preset([=]() -> f::Poolable* {
    auto ent= this->reifyNode("Explosion");
    auto sp = Explosion::create();
    CC_HIDE(sp);
    MGML()->addAtlasItem("game-pics", sp);
    ent->checkin(sp);
    return ent;
  }, count);
}

//////////////////////////////////////////////////////////////////////////
//
void GEngine::reifyBombs(int count) {
  auto p= MGMS()->getPool("Bombs");
  auto gz= XCFG()->gameSize();
  auto wz= cx::visSize();

  p->preset([=]() -> f::Poolable* {
    auto c = f::CPixie::reifyFrame("bomb.png");
    auto mv= mc_new(f::CMove);
    auto ent= this->reifyNode("Bomb");
    CC_HIDE(c);
    mv->speed.y= -50.0 * wz.height / gz.height;
    MGML()->addAtlasItem("game-pics", c);
    ent->checkin(mc_new(f::CHealth));
    ent->checkin(mv);
    ent->checkin(c);
    return ent;
  }, count);
}

//////////////////////////////////////////////////////////////////////////
//
const CCT_SZ GEngine::getRankInfo(GVars *ss, int r, c::Dictionary *out) {
  sstr s0 = "purple_bug_0.png";
  sstr s1= "purple_bug_1.png";
  int v;
  CCT_SZ z;

  if (r < 3) {
    v= 100;
    s0 = "blue_bug_0.png";
    s1 = "blue_bug_1.png";
    z= ss->blue;
  }
  else
  if (r < 5) {
    v= 50;
    s0 = "green_bug_0.png";
    s1= "green_bug_1.png";
    z = ss->green;
  } else {
    //s0 = "purple_bug_0.png";
    //s1= "purple_bug_1.png";
    z= ss->purple;
    v=30;
  }

  out->setObject(f::Size2::create(z), "size");
  out->setObject(CC_INT(v), "value");
  out->setObject(CC_STR(s0), "img0");
  out->setObject(CC_STR(s1), "img1");

  return z;
}

//////////////////////////////////////////////////////////////////////////
//
void GEngine::fillSquad(GVars *ss, f::FPool *pool) {

  auto rows = CC_CSV(c::Integer, "ROWS");
  auto cols = CC_CSV(c::Integer, "COLS");
  auto gz= XCFG()->gameSize();
  auto wz= cx::visSize();
  auto wb= cx::visBox();
  auto info= CC_DICT();
  float x,y;

  for (auto r=0; r < rows; ++r) {
    auto az = getRankInfo(ss, r, info);
    auto v = CC_GDI(info, "value");
    y = (r == 0) ? wb.top * 0.9
         : y - az.height - wz.height * 4/gz.height;
    x = wb.left + (8/gz.width * wz.width) + HWZ(az);
    for (auto c=0; c < cols; ++c) {
      auto aa = f::CPixie::reifyFrame(CC_GDS(info, "img0"));
      auto anim= cx::createAnimation(1,false,0);
      auto ent= this->reifyNode("Alien", true);
      anim->addSpriteFrame(
          cx::getSpriteFrame(CC_GDS(info, "img0")));
      anim->addSpriteFrame(
          cx::getSpriteFrame(CC_GDS(info, "img1")));
      aa->runAction(
          c::RepeatForever::create(c::Animate::create(anim)));
      MGML()->addAtlasItem("game-pics", aa);
      ent->checkin(mc_new2(Rank, v, r));
      ent->checkin(mc_new(f::CHealth));
      ent->checkin(aa);
      aa->inflate(x + HWZ(az), y - HHZ(az));
      pool->checkin(ent);
      x += az.width + (8/gz.width * wz.width);
    }
  }
}



NS_END


