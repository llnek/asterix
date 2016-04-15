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
NS_BEGIN(dttower)

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {
  auto ent= this->reifyNode("Shared",true);
  auto wz= cx::visSize();
  auto ss= mc_new(GVars);
  ent->checkin(ss);

  // global
  ss->squareSize = wz.height / 8;
  ss->waveNumber= -1;
  loadPathSteps(ss);

  // make some enemies
  auto po= MGMS()->reifyPool("Enemies");
  po->preset([=]() -> f::Poolable * {
    auto e= this->reifyNode("Enemy");
    auto n= Enemy::create(ss);
    MGML()->addAtlasItem("game-pics",n);
    CC_HIDE(n);
    e->checkin(mc_new(f::CHealth));
    e->checkin(mc_new(f::CMove));
    e->checkin(n);
    return e;
  }, 16);

  // make some defenses
  po= MGMS()->reifyPool("Defense1");
  po->preset([=]() -> f::Poolable *  {
    auto e= this->reifyNode("Defense1");
    auto n= Defense::create(ss, levelOne);
    MGML()->addAtlasItem("game-pics",n);
    CC_HIDE(n);
    e->checkin(n);
    return e;
  }, 16);
  po= MGMS()->reifyPool("Defense2");
  po->preset([=]() -> f::Poolable * {
    auto e= this->reifyNode("Defense2");
    auto n= Defense::create(ss, levelTwo);
    MGML()->addAtlasItem("game-pics",n);
    CC_HIDE(n);
    e->checkin(n);
    return e;
  }, 16);

  // tower = player
  auto tower= f::CPixie::reifyFrame("tower.png");
  auto pos= ss->pathSteps.back()->getPosition();
  ent= this->reifyNode("Tower", true);
  tower->setPosition(pos);
  MGML()->addAtlasItem("game-pics", tower, 2);
  ent->checkin(mc_new1(f::CHealth,10));
  ent->checkin(mc_new(f::CHuman));
  ent->checkin(tower);

}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::loadPathSteps(GVars *ss) {
  auto wb= cx::visBox();
  s_arr<CCT_PT,10> pts = {
    CCT_PT(-50, wb.top - ss->squareSize * 3.30),
    CCT_PT(ss->squareSize * 2.5, wb.top - ss->squareSize * 3.3),
    CCT_PT(ss->squareSize * 2.5, ss->squareSize * 2),
    CCT_PT(ss->squareSize * 5.2, ss->squareSize * 2),
    CCT_PT(ss->squareSize * 5.2, wb.top - ss->squareSize*2),
    CCT_PT(ss->squareSize * 7.8, wb.top - ss->squareSize*2),
    CCT_PT(ss->squareSize * 7.8, ss->squareSize * 3.5),
    CCT_PT(ss->squareSize * 10.5, ss->squareSize * 3.5),
    CCT_PT(ss->squareSize * 10.5, wb.top - ss->squareSize*2),
    CCT_PT(wb.right - ss->squareSize * 1.2, wb.top - ss->squareSize * 2)
  };

  ss->pathSteps.clear();

  PathStep *prev= CC_NIL;
  F__LOOP(it, pts) {
    auto &pt= *it;
    auto p= PathStep::create(pt);
    ss->pathSteps.push_back(p);
    MGML()->addAtlasItem("game-pics",p);
    CC_HIDE(p);
    if (!prev) {
      prev=p;
    } else {
      prev->next = p;
      prev=p;
    }
  }

  //debug
  prev=ss->pathSteps.front();
  while (prev) {
    auto pos= prev->getPosition();
    prev=prev->next;
    CCLOG("pathstep.pos = %d,%d", (int)pos.x, (int)pos.y);
  }
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

