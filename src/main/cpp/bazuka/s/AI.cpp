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

//////////////////////////////////////////////////////////////////////////////

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "AI.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(bazuka)

//////////////////////////////////////////////////////////////////////////////
//
void AI::preamble() {
  shared=engine->getNodeList(SharedNode().typeId());
  createEnemyTimer();
}

//////////////////////////////////////////////////////////////////////////////
//
bool AI::update(float dt) {
  if (MGMS()->isLive()) {
    parallex(dt);
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::parallex(float dt) {
  auto ss= CC_GNLF(GVars, shared, "slots");
  auto wb= cx::visBox();

  ss->bgLayer->sync();
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::process(float dt) {

  if (!cx::timerDone(timer)) { return; }
  else {
    cx::undoTimer(timer);
  }

  auto p= MGMS()->getPool("Enemies");
  auto tmp= (Enemy*) p->getAndSet(true);
  auto wb= cx::visBox();
  auto sz= tmp->csize();

  auto mrand = 1 + (rand() % 3);
  float h = wb.top * mrand * 0.25f ;
  CCPoint p = ccp(visibleSize.width + this->getContentSize().width/2 , h);
  this->setPosition(p);
  auto idleanimation = c::Animation::create();
  for (auto n = 1; n <= 4;  ++n) {
    idleanimation->addSpriteFrame(
        cx::getSpriteFrame("enemy_idle_" + s::to_string(n) + ".png"));
  }
  idleanimation->setDelayPerUnit(0.25f);
  s->runAction(c::RepeatForever::create(c::Animate::create(idleanimation))) ;



  createEnemyTimer();
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::createEnemyTimer() {
  auto t1= CC_CSV(c::Integer, "ADD+ENEMY+DELAY");
  timer= cx::reifyTimer(MGML(), t1);
}


NS_END



