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
#include "n/Enemy.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(bazuka)

//////////////////////////////////////////////////////////////////////////////
//
void AI::preamble() {
  shared=engine->getNodeList(SharedNode().typeId());
  spawnEnemyTimer();
}
//////////////////////////////////////////////////////////////////////////////
//
void AI::spawnEnemyTimer() {
  enemyTimer= cx::reifyTimer(MGML(), CC_CSV(c::Integer, "ENEMY+SPAWN+DELAY"));
}

//////////////////////////////////////////////////////////////////////////////
//
bool AI::update(float dt) {
  if (MGMS()->isLive()) {
    parallex(dt);
    processEnemies(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::parallex(float dt) {
  auto ss= CC_GNLF(GVars, shared, "slots");
  auto wb= cx::visBox();


}

//////////////////////////////////////////////////////////////////////////////
//
void AI::processEnemies(float dt) {

  if (!cx::timerDone(enemyTimer)) { return; }
  else {
    cx::undoTimer(enemyTimer);
  }

  auto po= MGMS()->getPool("Enemies");
  auto e= (Enemy*)po->getAndSet(true);
  auto mrand = 1 + (rand()%3);
  auto wz= cx::visRect();
  auto wb= cx::visBox();
  auto sz= e->csize();

  e->inflate(wb.right + HWZ(sz), wz.size.height * mrand * 0.25);
  e->lockAndLoad();

  spawnEnemyTimer();
}



NS_END



