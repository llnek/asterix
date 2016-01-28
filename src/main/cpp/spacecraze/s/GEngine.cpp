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
#include "Aliens.h"
#include "Move.h"
#include "GEngine.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(spacecraze)


//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {
  MGMS()->reifyPool("Missiles");
  MGMS()->reifyPool("Bombs");
  MGMS()->reifyPool("Aliens");

  createMissiles();
  createBombs();

  createAliens();
  createShip();
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createMissiles(int cnt) {
  auto p= MGMS()->getPool("Missiles");
  auto g= this->getCfg()["player"];
  auto d= JS_INT(g["speed"]);
  p->preset([=]() {
    auto sp=cx::reifySprite("sfbullet");
    auto c= mc_new1(Missile,sp);
    c->speed.y=d;
    c->vel.y=d;
    MGML()->addAtlasItem("game-pics", sp);
    return c;
  },cnt);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createBombs(int cnt) {
  auto p= MGMS()->getPool("Bombs");
  auto g= this->getCfg()["enemy"];
  auto d= JS_INT(g["speed"]);
  p->preset([=]() {
    auto sp=c::Sprite::create();
    auto c= mc_new1(Bomb,sp);
    c->speed.y=d;
    c->vel.y=d;
    c->morph(1);
    MGML()->addAtlasItem("game-pics", sp);
    return c;
  },cnt);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initSystems() {
  regoSystem(mc_new1(Aliens,this));
  regoSystem(mc_new1(Move,this));
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createShip() {

  auto p = cx::reifySprite("sfgun");
  auto ent= this->reifyEntity();
  auto z= p->getContentSize();
  auto c= mc_new1(Ship,p);
  auto t= mc_new(Looper);
  auto cfg= this->getCfg();
  auto d= cfg["player"];

  t->duration = JS_FLOAT(d["fireRate"]) * 1000;
  t->timer = cx::reifyTimer(MGML(), t->duration);

  ent->checkin(mc_new(Gesture));
  ent->checkin(t);
  ent->checkin(c);

  MGML()->addAtlasItem("game-pics", p);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createAliens() {
  auto pool= MGMS()->getPool("Aliens");
  auto mez = cx::calcSize("sfenmy3");
  auto map= this->getCfg();
  auto wz= cx::visRect();
  auto wb= cx::visBox();
  auto top = wb.top * 0.8f;
  auto enemies= map["enemy"];
  auto dur= JS_FLOAT(enemies["moveDuration"]);
  auto r2= JS_FLOAT(enemies["fireRate"]);
  auto scores= enemies["scores"];
  auto layout= enemies["layout"];
  auto maxRight=wb.left;
  auto minLeft=wb.right;
  J__LOOP(it,layout) {
    auto obj = *it;
    float height_t=0;
    float width_t=0;
    auto gap= JS_FLOAT(obj["gap"]);
    j::json fmt= JS_ARR(obj["fmt"]);
    s_vec<Alien*> aliens;
    J__LOOP(it2, fmt) {
      auto n = *it2;
      auto a = new Alien(JS_INT(n));
      auto sz= a->csize();
      width_t += sz.width;
      height_t= MAX(sz.height, height_t);
      aliens.push_back(a);
    }
    width_t += gap * (aliens.size()-1);

    auto lf= wb.left + 0.5f * (wb.right - wb.left - width_t);
    auto rt= lf + width_t;
    maxRight=MAX(maxRight, rt);
    minLeft=MIN(minLeft,lf);
    F__LOOP(it,aliens) {
      auto a= *it;
      auto sw= a->csize().width;
      a->inflate(lf + sw * 0.5f, top);
      lf += sw + gap;
      pool->checkin(a);
    }
    top -= height_t * 1.5f;
  }

  auto q= mc_new1(AlienSquad,pool);
  auto ent= this->reifyEntity();
  auto lpr= mc_new(Looper);

  q->rightEdge= maxRight;
  q->leftEdge= minLeft;
  q->duration=dur;

  lpr->duration= r2 * 1000;
  lpr->timer=cx::reifyTimer(MGML(), lpr->duration);

  ent->checkin(q);
  ent->checkin(lpr);
}


NS_END




