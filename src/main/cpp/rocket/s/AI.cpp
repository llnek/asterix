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
NS_BEGIN(rocket)

//////////////////////////////////////////////////////////////////////////////
//
void AI::preamble() {
  drawings=engine->getNodeList(LineDrawingNode().typeId());
  shared=engine->getNodeList(SharedNode().typeId());
  rockets=engine->getNodeList(RocketNode().typeId());
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
  auto rock=CC_GNLF(Rocket,rockets,"rocket");
  auto ss=CC_GNLF(GVars,shared,"slots");
  auto wb=cx::visBox();

  //update timers
  ss->cometTimer += dt;
  if (ss->cometTimer > ss->cometInterval) {
    ss->cometTimer = 0;
    if (!ss->comet->isVisible()) {
      ss->comet->setPositionX(0);
      auto newY = (float)rand()/((float)RAND_MAX/wb.top * 0.6f) + wb.top * 0.2f;
      if (newY > wb.top * 0.9f) { newY = wb.top * 0.9f; }
      ss->comet->setPositionY(newY);
      CC_SHOW(ss->comet);
      ss->comet->resetSystem();
    }
  }

  if (ss->comet->isVisible()) {
    //collision with comet
    if (pow(ss->comet->getPositionX() - rock->sprite->getPositionX(), 2) +
        pow(ss->comet->getPositionY() - rock->sprite->getPositionY(), 2) <= pow (rock->radius() , 2)) {
      if (rock->sprite->isVisible()) {
        SENDMSG("/game/player/killed");
      }
    }
    ss->comet->setPositionX(ss->comet->getPositionX() + 50 * dt);
    if (ss->comet->getPositionX() > wb.right * 1.5f) {
      ss->comet->stopSystem();
      CC_HIDE(ss->comet);
    }
  }


}




NS_END



