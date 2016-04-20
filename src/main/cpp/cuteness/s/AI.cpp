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
NS_BEGIN(cuteness)

//////////////////////////////////////////////////////////////////////////////
//
void AI::preamble() {
  _enemies= _engine->getNodes("n/ESquad")[0];
  _planet= _engine->getNodes("f/CHuman")[0];
  _shared= _engine->getNodes("n/GVars")[0];
  _timer= cx::reifyTimer(MGML(), 1000);
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
  auto ss=CC_GEC(GVars, _shared, "n/GVars");
  if (! ss->enabled) { return; }
  if (!cx::timerDone(_timer)) { return;}
  cx::undoTimer(_timer);
  _timer=CC_NIL;
  auto sq= CC_GEC(EnemySquad,_enemies,"n/ESquad");
  auto pt= CC_GEC(Planet,_planet,"f/CPixie");
  float x= c::random(10,100) * cx::randSign();
  auto wb= cx::visBox();
  float y = cx::randInt(wb.top);
  auto po= sq->randGet();
  auto e= po->take(true);
  auto sp= CC_GEC(Enemy,e,"f/CPixie");
  if (x > 0) { x += wb.right; }
  cx::resurrect((ecs::Node*)e, x, y);
  auto t= c::random(2500-ss->speedBump, 4500-ss->speedBump) / 1000;
  sp->runAction(
      c::MoveTo::create(t, pt->getPosition()));
  ss->speedBump += 50;
  _timer=cx::reifyTimer(MGML(), 1000);
}



NS_END



