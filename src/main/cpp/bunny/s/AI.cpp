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

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(bunny)

//////////////////////////////////////////////////////////////////////////////
//
void AI::preamble() {
  _shared= _engine->getNodes("n/GVars")[0];
  _timerB= cx::reifyTimer(MGML(), 8000);
  _timer= cx::reifyTimer(MGML(), 3000);
  dropBombs(1,-100);
}

//////////////////////////////////////////////////////////////////////////////
//
bool AI::update(float dt) {
  if (MGMS()->isLive()) {
    processScore(dt);
    processBombs(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::processScore(float dt) {

  if (!cx::timerDone(_timer)) { return; }
  else {
    cx::undoTimer(_timer);
  }

  SENDMSG("/game/hud/updatescore");

  _timer= cx::reifyTimer(MGML(), 3000);
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::processBombs(float dt) {

  if (!cx::timerDone(_timerB)) { return; }
  else {
    cx::undoTimer(_timerB);
  }

  dropBombs(3,-250);
  _timerB= cx::reifyTimer(MGML(), 8000);
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::dropBombs(int count, float v) {

  auto po= MGMS()->getPool("Bombs");
  auto wb= cx::visBox();

  for (auto i = 0 ; i < count ; ++i) {
    auto p= po->take(true);
      auto b=CC_GEC(f::CPixie,p,"f/CPixie");
    auto sp= PCAST(c::Sprite,b);
    auto sz= CC_CSIZE(sp);
    auto pb= sp->getPhysicsBody();
    pb->setVelocity(c::Vec2(0, (cx::rand() + 0.2) * v));
    pb->setEnabled(true);
    b->inflate(cx::randInt(wb.right), wb.top + HHZ(sz));
  }

}



NS_END



