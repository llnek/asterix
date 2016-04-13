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

//////////////////////////////////////////////////////////////////////////////

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "AI.h"
#include "n/Enemy.h"
#include "n/Defense.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(dttower)

//////////////////////////////////////////////////////////////////////////////
//
void AI::preamble() {
  _shared= _engine->getNodes("n/GVars")[0];
}

//////////////////////////////////////////////////////////////////////////////
//
bool AI::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::process(float dt) {
  loadEnemy(dt);
  loadWave(dt);
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::loadEnemy(float dt) {
  if (!cx::timerDone(_timer2)) {
  return; }

  cx::undoTimer(_timer2);
  _timer2=CC_NIL;

  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto po=MGMS()->getPool("Enemies");

  if (ss->countEnemies < WAVES_NUM_ENEMIES) {
    auto e= po->take(true);
    auto n= CC_GEC(Enemy,e,"f/CPixie");
    cx::resurrect((ecs::Node*)e);
    n->set(ss->pathSteps[0] );
    ss->countEnemies += 1;
    _timer2= cx::reifyTimer(MGML(), 1000);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::loadWave(float dt) {
    auto ss=CC_GEC(GVars,_shared,"n/GVars");
  if (ss->waveNumber < 0) {
    ss->waveNumber= 0;
  }
  else
  if (!cx::timerDone(_timer1)) {
  return; }

  cx::undoTimer(_timer1);
  _timer1=CC_NIL;

  if (ss->waveNumber > NUM_WAVES) {
  return; }
  ss->waveNumber += 1;

  auto msg= j::json({
        {"wave", ss->waveNumber}
      });

  SENDMSGEX("/game/hud/updatewave", &msg);
  ss->countEnemies = 0;

  _timer2= cx::reifyTimer(MGML(), 1000);
  _timer1= cx::reifyTimer(MGML(), WAVES_INTERVAL*1000);
}


NS_END



