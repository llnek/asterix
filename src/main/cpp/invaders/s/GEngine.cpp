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
  reifyArena();
  reifyAliens();
  reifyShip();
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
ecs::Node* GEngine::reifyArena() {
  //pick purple since it is the largest
  auto z0= cx::calcSize("purple_bug_0.png");
  auto s0= cx::calcSize("ship_0.png");
  auto ent= this->reifyNode();
  auto ss = mc_new(GVars);

  ss->alienSize = z0;
  ss->shipSize = s0;
  ent->checkin(ss);

  return ent;
}

//////////////////////////////////////////////////////////////////////////
//
void GEngine::reifyMissiles(int count) {
  auto p= MGMS()->getPool("Missiles");
  auto gz= XCFG()->gameSize();
  auto wz= cx::visRect();
  p->preset([=]() -> f::Poolable* {
    auto sp = cx::reifySprite("missile.png");
    auto mv= mc_new(f::CMove);
    CC_HIDE(sp);
    mv->speed= 150.0 * wz.size.height / gz.height;
    MGML()->addAtlasItem("game-pics", sp);
    auto ent= this->reifyNode();
    ent->checkin(mc_new1(f::CDraw, sp));
    ent->checkin(mc_new(f::CHealth));
    ent->checkin(mv);
    return ent;
  }, count);
}

//////////////////////////////////////////////////////////////////////////
//
void GEngine::reifyExplosions(int count) {
  auto p= MGMS()->getPool("Explosions");
  p->preset([=]() -> f::Poolable* {
    auto sp = cx::reifySprite("boom_0.png");
    auto ent= this->reifyNode();
    CC_HIDE(sp);
    MGML()->addAtlasItem("game-pics", sp);
    ent->checkin(mc_new1(Explosion, sp));
    return ent;
  }, count);
}

//////////////////////////////////////////////////////////////////////////
//
void GEngine::reifyBombs(int count) {
  auto p= MGMS()->getPool("Bombs");
  auto gz= XCFG()->gameSize();
  auto wz= cx::visRect();
  p->preset([=]() -> f::Poolable* {
    auto c = new f::CDraw(cx::reifySprite("bomb.png"));
    auto mv= mc_new(f::CMove);
    auto ent= this->reifyNode();
    c->hide();
    mv->speed= -50.0 * wz.size.height / gz.height;
    MGML()->addAtlasItem("game-pics", c->node);
    ent->checkin(mc_new(f::CHealth));
    ent->checkin(mv);
    ent->checkin(c);
    return ent;
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
void GEngine::fillSquad(f::FPool *pool) {

  auto rows = CC_CSV(c::Integer, "ROWS");
  auto cols = CC_CSV(c::Integer, "COLS");
  auto gz= XCFG()->gameSize();
  auto wz= cx::visRect();
  auto wb= cx::visBox();
  auto info= CC_DICT();
  float x,y;

  for (auto r=0; r < rows; ++r) {
    auto az = getRankInfo(r, info);
    auto v = CC_GDV(c::Integer, info, "value");
    y = (r == 0) ? wb.top * 0.9
         : y - az.height - wz.size.height * 4/gz.height;
    x = wb.left + (8/gz.width * wz.size.width) + HWZ(az);
    for (auto c=0; c < cols; ++c) {
      auto aa = new f::CDraw(cx::reifySprite(CC_GDS(info, "img0")));
      auto anim= c::Animation::create();
      auto f1= CC_GDS(info, "img0");
      auto f2= CC_GDS(info, "img1");
      auto ent= this->reifyNode();
      anim->addSpriteFrame( cx::getSpriteFrame(f1));
      anim->addSpriteFrame( cx::getSpriteFrame(f2));
      anim->setDelayPerUnit(1);
      aa->node->runAction(
          c::RepeatForever::create(c::Animate::create(anim)));
      MGML()->addAtlasItem("game-pics", aa->node);
      ent->checkin(mc_new2(Rank, v, r));
      ent->checkin(mc_new(f::CHealth));
      ent->checkin(aa);
      aa->inflate(x + HWZ(az), y - HHZ(az));
      ent->take();
      pool->checkin(ent);
      x += az.width + (8/gz.width * wz.size.width);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
ecs::Node* GEngine::reifyAliens() {
  // use the largest one to calc size
  auto z0= cx::calcSize("purple_bug_0.png");
  auto stepx= z0.width / 3.0;
  auto ent= this->reifyNode();
  auto p = MGMS()->getPool("Aliens");

  fillSquad(p);

  ent->checkin(mc_new2(AlienSquad, p, stepx));
  ent->checkin(mc_new(f::Loopers));
  return ent;
}

//////////////////////////////////////////////////////////////////////////
//
ecs::Node* GEngine::reifyShip() {

  auto s= cx::reifySprite("ship_1.png");
  auto sz= cx::calcSize("ship_0.png");
  auto ent= this->reifyNode();
  auto wz= cx::visRect();
  auto wb= cx::visBox();

  //TODO: why 60
  auto y = sz.height + wb.bottom + (5/60.0 * wz.size.height);
  auto ship = mc_new3(Ship, s, "ship_0.png", "ship_1.png");
  auto mv= mc_new(f::CMove);

  MGML()->addAtlasItem("game-pics", s);
  ship->inflate(wb.cx,y);
  mv->speed=150;

  ent->checkin(mc_new1(Cannon,true));
  ent->checkin(mc_new(f::CHealth));
  ent->checkin(mc_new(f::Looper));
  ent->checkin(ship);
  ent->checkin(mv);
  ent->take();

  return ent;
}



NS_END


