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

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Move.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(breakout)

//////////////////////////////////////////////////////////////////////////////
//
void Move::preamble() {
  paddle = engine->getNodeList(PaddleMotionNode().typeId());
  ball = engine->getNodeList( BallMotionNode().typeId());
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
  auto ba= CC_GNLF(Ball, ball, "ball");
  auto B = MGMS()->getEnclosureBox();
  auto pos= ba->pos();
  auto rect= cx::bbox4(ba);

  c::Vec2 outPos;
  c::Vec2 outVel;
  bool rc=cx::traceEnclosure(dt, B,
                         rect,
                         ba->vel,
                         outPos, outVel);

  ba->vel.x = outVel.x;
  ba->vel.y = outVel.y;
  ba->setPos(outPos.x, outPos.y);
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::processPaddleMotions(float dt) {
  auto motion = CC_GNLF(Gesture,paddle,"motion");
  auto pad = CC_GNLF(Paddle,paddle,"paddle");
  auto pos = pad->pos();
  auto x= pos.x;
  auto y= pos.y;

  if (MGML()->keyPoll(KEYCODE::KEY_RIGHT_ARROW)) {
    x = pos.x + dt * pad->vel.x;
  }

  if (MGML()->keyPoll(KEYCODE::KEY_LEFT_ARROW)) {
    x = pos.x - dt * pad->vel.x;
  }

  pad->setPos(x,y);
  clamp(pad);

}

//////////////////////////////////////////////////////////////////////////////
//
void Move::clamp(Paddle *pad) {
  auto sz= pad->sprite->getContentSize();
  auto tile = CC_CSV(c::Float, "TILE");
  auto pos= pad->sprite->getPosition();
  auto wz = cx::visRect();

  if (cx::getRight(pad->sprite) > wz.size.width - tile) {
    pad->setPos(wz.size.width - tile - sz.width * 0.5f, pos.y);
  }

  if (cx::getLeft(pad->sprite) < tile) {
    pad->setPos( tile + sz.width * 0.5f, pos.y);
  }
}

NS_END(breakout)


