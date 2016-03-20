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
  _shared= _engine->getNodes("n/GVars")[0];
  _timer= cx::reifyTimer(MGML(), 1500);
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
  auto ss= CC_GEC(GVars, _shared, "n/GVars");
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

  if (!cx::timerDone(_timer)) { return; }
  else {
    cx::undoTimer(_timer);
  }

  auto po= MGMS()->getPool("Asteroids");
  auto e= (ecs::Node*)po->take(true);
    auto r= (c::Sprite*)CC_GEC(f::CPixie,e,"f/CPixie");
  auto wb= cx::visBox();
  auto sz= CC_CSIZE(r);
  auto rx = HWZ(sz) + cx::randInt(wb.right - CC_ZW(sz));

  cx::resurrect(e, wb.left + rx , wb.top + sz.height);
  r->getPhysicsBody()->setEnabled(true);

  _timer= cx::reifyTimer(MGML(), 1500);
}



NS_END



