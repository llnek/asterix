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
#include "Collide.h"
#include "Resolve.h"
#include "Aliens.h"
#include "Move.h"
#include "GEngine.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(spacecraze)


//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {
  MGMS()->reifyPools(
      s_vec<sstr>{"Missiles", "Bombs", "Aliens"});

  createMissiles();
  createBombs();
  createArena();
  createAliens();
  createShip();
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createArena() {
  auto ent= this->reifyNode("Arena",true);
  auto cfg= this->getCfg();
  auto ees= cfg["enemy"];
  auto p= cfg["player"];
  auto lpr= mc_new(f::Loopers);
  auto ss= mc_new(GVars);
  f::DLTimer t;

  // player
  t.duration = JS_FLOAT(p["fireRate"]) * 1000;
  t.timer= cx::reifyTimer(MGML(), t.duration);
  lpr.tms.push_back(t);

  // aliens
  t.duration= JS_FLOAT(ees["fireRate"]) * 1000;
  t.timer=cx::reifyTimer(MGML(), t.duration);
  lpr.tms.push_back(t);

  ent->checkin(lpr);
  ent->checkin(ss);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createMissiles(int cnt) {
  auto po= MGMS()->getPool("Missiles");
  auto g= this->getCfg()["player"];
  auto d= JS_INT(g["speed"]);
  po->preset([=]() {
    auto sp=cx::reifySprite("sfbullet");
    auto c= mc_new1(Missile,sp);
    auto mv=mc_new(f::CMove);
    auto ent= this->reifyNode("Missile");

    MGML()->addAtlasItem("game-pics", sp);
    mv->speed.y=d;
    mv->vel.y=d;
    c->hide();

    ent->checkin(mc_new(f::CHealth));
    ent->checkin(mv);
    ent->checkin(c);
    return ent;

  },cnt);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createBombs(int cnt) {
  auto po= MGMS()->getPool("Bombs");
  auto g= this->getCfg()["enemy"];
  auto d= JS_INT(g["speed"]);
  po->preset([=]() {
    auto sp=c::Sprite::create();
    auto c= mc_new1(Bomb,sp);
    auto mv=mc_new(f::CMove);
    auto ent=this->reifyNode("Bomb");

    MGML()->addAtlasItem("game-pics", sp);
    mv->speed.y=d;
    mv->vel.y=d;
    c->morph(1);
    c->hide();

    ent->checkin(mc_new(f::CHealth));
    ent->checkin(mv);
    ent->checkin(c);
    return ent;

  },cnt);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initSystems() {
  regoSystem(mc_new1(Aliens,this));
  regoSystem(mc_new1(Move,this));
  regoSystem(mc_new1(Collide,this));
  regoSystem(mc_new1(Resolve,this));

}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createShip() {

  auto ent= this->reifyNode("Ship",true);
  auto s= cx::reifySprite("sfgun");
  auto c= mc_new1(Ship,s);

  ent->checkin(mc_new(f::CGesture));
  ent->checkin(mc_new(f::CHealth));
  ent->checkin(mc_new(f::CMove));
  ent->checkin(c);

  MGML()->addAtlasItem("game-pics", s, 999);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createAliens() {
  auto pool= MGMS()->getPool("Aliens");
  auto mez = cx::calcSize("sfenmy3");
  auto map= this->getCfg();
  auto wz= cx::visRect();
  auto wb= cx::visBox();
  auto top = wb.top * 0.8;
  auto ees= map["enemy"];
  auto scores= ees["scores"];
  auto layout= ees["layout"];
  auto maxRight=wb.left;
  auto minLeft=wb.right;

  J__LOOP(it,layout) {
    auto obj = *it;
    float height_t=0;
    float width_t=0;
    j::json fmt= JS_ARR(obj["fmt"]);
    auto gap= JS_FLOAT(obj["gap"]);
    s_vec<Alien*> aliens;
    J__LOOP(it2, fmt) {
      auto n = *it2;
      auto a = new Alien(JS_INT(n));//,1,10);
      auto sz= a->csize();
      width_t += sz.width;
      height_t= MAX(sz.height, height_t);
      aliens.push_back(a);
    }
    width_t += gap * (aliens.size()-1);

    auto lf= wb.left + 0.5 * (wb.right - wb.left - width_t);
    auto rt= lf + width_t;
    maxRight=MAX(maxRight, rt);
    minLeft=MIN(minLeft,lf);
    F__LOOP(it,aliens) {
      auto e= this->reifyNode("Alien",true);
      auto a= *it;
      auto sz= a->csize();
      a->inflate(lf + HWZ(sz), top);
      lf += sz.width + gap;
      e->checkin(mc_new1(f::CStats, 10));
      e->checkin(mc_new(f::CHealth));
      e->checkin(mc_new(f::CMove));
      e->checkin(a);
      pool->checkin(e);
    }
    top -= height_t * 1.5;
  }

  auto ent= this->reifyNode("AlienSquad");
  auto q= mc_new(AlienSquad);
  q->rightEdge= maxRight;
  q->leftEdge= minLeft;
  q->duration= JS_FLOAT(ees["moveDuration"]);

  ent->checkin(q);
}


NS_END




