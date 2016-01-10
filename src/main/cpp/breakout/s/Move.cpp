// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2015, Ken Leung. All rights reserved.

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Move.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(breakout)

//////////////////////////////////////////////////////////////////////////////
//
void Move::initEntities() {

}

//////////////////////////////////////////////////////////////////////////////
//
void Move::initSystems() {


}

//////////////////////////////////////////////////////////////////////////////
//
void Move::preamble() {
  paddleNode = engine->getNodeList(PaddleMotionNode().typeId());
  ballNode = engine->getNodeList( BallMotionNode().typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Move::update(float dt) {
  if (MGMS()->isLive()) {
    processBallMotions( dt);
    processPaddleMotions(dt);
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::processBallMotions(float dt) {
  auto ball = CC_GNLF(Ball, ballNode, "ball");
  auto B = MGMS()->getEnclosureBox();
  auto pos= ball->pos();
  auto rect= cx::bbox(ball->sprite);

  rect.x = pos.x;
  rect.y = pos.y;

  c::Vec2 outPos;
  c::Vec2 outVel;
  bool rc=cx::traceEnclosure(dt, B,
                         rect,
                         ball->vel,
                         outPos, outVel);

  ball->vel.x = outVel.x;
  ball->vel.y = outVel.y;
  ball->setPos(outPos.x, outPos.y);
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::processPaddleMotions(float dt) {
  auto motion = CC_GNLF(Motion,paddleNode,"motion");
  auto pad = CC_GNLF(Paddle,paddleNode,"paddle");
  auto pos = pad->pos();
  auto x= pos.x;
  auto y= pos.y;

  if (motion->right) {
    x = pos.x + dt * pad->vel.x;
  }

  if (motion->left) {
    x = pos.x - dt * pad->vel.x;
  }

  pad->setPos(x,y);
  clamp(pad);

  motion->right=false;
  motion->left=false;
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::clamp(Paddle *pad) {
  auto sz= pad->sprite->getContentSize();
  auto tile = CC_CSV(c::Integer, "TILE");
  auto pos= pad->sprite->getPosition();
  auto wz = cx::visRect();

  if (cx::getRight(pad->sprite) > wz.size.width - tile) {
    pad->setPos(wz.size.width - tile - sz.width * 0.5f, pos.y);
  }

  if (cx::getLeft(pad->sprite) < tile) {
    pad->setPos( tile + sz.height * 0.5f, pos.y);
  }
}

NS_END(breakout)


