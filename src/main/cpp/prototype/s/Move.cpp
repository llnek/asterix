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
#include "Move.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(prototype)


//////////////////////////////////////////////////////////////////////////////
//
void Move::preamble() {
  players=engine->getNodeList(PlayerNode().typeId());
  shared=engine->getNodeList(SharedNode().typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Move::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::process(float dt) {
  auto ss= CC_GNLF(GVars, shared, "slots");
  auto p= MGMS()->getPool("Asteroids");
  auto &c=p->list();
  auto wb= cx::visBox();

  F__LOOP(it,c) {
    auto a= (Asteroid*) *it;
    if (!a->status) { continue; }
    a->setPos(a->node->getPosition().x, a->node->getPosition().y + (0.75 * wb.top * dt));
    if (a->node->getPosition().y > wb.top * 1.2) {
      a->node->getPhysicsBody()->setEnabled(false);
      a->deflate();
    }
  }

  if (ss->isTouching) {
    onTouch(dt, ss->touchPt);
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void Move::onTouch(float dt, const c::Vec2 &loc) {

  auto py= CC_GNLF(SpaceShip, players, "player");
  auto wz= cx::visRect();
  auto wb= cx::visBox();
  auto sz= py->csize();
  auto delta= HWZ(wz.size) * dt;

  if  (loc.x < wb.cx) {
    py->node->setPositionX(py->node->getPosition().x - delta);
    if (py->node->getPosition().x <= wb.left + HWZ(sz)) {
      py->node->setPositionX(wb.left + HWZ(sz));
    }
  } else {
    py->node->setPositionX(py->node->getPosition().x + delta);
    if (py->node->getPosition().x >= wb.right - HWZ(sz)) {
      py->node->setPositionX(wb.right - HWZ(sz));
    }
  }

}


NS_END




