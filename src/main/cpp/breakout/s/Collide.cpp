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

#include "Collide.h"

//////////////////////////////////////////////////////////////////////////////
//
void Collide::preamble() {
  paddleNode = engine->getNodeList(PaddleMotionNode().typeId());
  ballNode = engine->getNodeList(BallMotionNode().typeId());
  fenceNode= engine->getNodeList(BricksNode().typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Collide::update(float dt) {
  if (MGMS()->isLive()) {
    if (! onPlayerKilled()) {
      checkNodes();
      checkBricks();
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
bool Collide::onPlayerKilled() {
  auto pad= CC_GNLF(Paddle,paddleNode,"paddle");
  auto ball = CC_GNLF(Ball,ballNode,"ball");
  auto pos= ball->pos();

  if (pos.y < cx::getBottom(pad->sprite)) {
    SENDMSG("/game/players/killed");
    return true;
  } else {
    return false;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::checkNodes() {
  auto pad= CC_GNLF(Paddle,paddleNode,"paddle");
  auto ball = CC_GNLF(Ball,ballNode,"ball");

  if (cx::collide(pad,ball)) {
    check();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::check() {
  auto pad= CC_GNLF(Paddle,paddleNode,"paddle");
  auto ball = CC_GNLF(Ball,ballNode,"ball");
  auto sz= ball->sprite->getContentSize();
  auto hh= sz.height * 0.5f;
  auto pos= ball->pos();
  auto top= cx::getTop(pad->sprite);

  ball->setPos(pos.x, top+hh);
  ball->vel.y = - ball->vel.y;
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::checkBricks() {
  auto fence= CC_GNLF(BrickFence,fenceNode,"fence");
  auto ball = CC_GNLF(Ball,ballNode,"ball");
  auto &bss= fence->bricks;

  for (auto n=0; n < bss.size(); ++n) {
    if (! bss[n]->status) { continue; }
    if (cx::collide(ball->sprite, bss[n]->sprite)) {
      onBrick(bss[n]);
      break;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::onBrick(Brick *brick) {
  auto ball = CC_GNLF(Ball,ballNode,"ball");
  auto bz = ball->sprite->getContentSize();
  auto kz= brick->sprite->getContentSize();
  auto ks= brick->sprite;
  auto bs= ball->sprite;
  auto ka= cx::bbox4(ks);
  auto ba = cx::bbox4(bs);

  // ball coming down from top?
  if (ba.top > ka.top &&  ka.top > ba.bottom) {
    ball->vel.y = - ball->vel.y;
  }
  else
  // ball coming from bottom?
  if (ba.top > ka.bottom &&  ka.bottom > ba.bottom) {
    ball->vel.y = - ball->vel.y;
  }
  else
  // ball coming from left?
  if (ka.left > ba.left && ba.right > ka.left) {
    ball->vel.x = - ball->vel.x;
  }
  else
  // ball coming from right?
  if (ka.R > ba.L && ba.R > ka.R) {
    ball->vel.x = - ball->vel.x;
  }
  else {
    throw "Failed to determine the collide of ball and brick.";
  }

  auto msg= j::json({
      {"value", brick->score }
      });
  SENDMSGEX("/game/players/earnscore", &msg);
  brick->deflate();
}

NS_END(breakout)


