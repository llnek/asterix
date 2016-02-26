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
NS_BEGIN(prototype)

//////////////////////////////////////////////////////////////////////////////
//
void AI::preamble() {
  shared=engine->getNodeList(SharedNode().typeId());
  timer= cx::reifyTimer(MGML(), 1500);
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

  F__LOOP(it, ss->bgSprites) {
    auto s= *it;
    if (s->getPosition().y >= wb.top + wb.cy - 1) {
      s->setPosition(wb.cx, (-1 * wb.top) + wb.cy);
    }
  }

  F__LOOP(it, ss->bgSprites) {
    auto s= *it;
    s->setPosition(
        s->getPosition().x,
        s->getPosition().y + (0.75 * wb.top * dt));
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void AI::process(float dt) {

  if (!cx::timerDone(timer)) { return; }
  else {
    cx::undoTimer(timer);
  }

  auto p= MGMS()->getPool("Asteroids");
  auto tmp= (Asteroid*) p->getAndSet(true);
  auto wb= cx::visBox();
  auto sz= tmp->csize();
  auto rx = HWZ(sz) + cx::randInt( wb.right - sz.width );

  tmp->inflate(wb.left + rx , wb.bottom + sz.height);
  tmp->node->getPhysicsBody()->setEnabled(true);
  timer= cx::reifyTimer(MGML(), 1500);
}



NS_END



