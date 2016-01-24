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
#include "Collide.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(breakout)

//////////////////////////////////////////////////////////////////////////////
//
void Collide::preamble() {
  paddle = engine->getNodeList(PaddleMotionNode().typeId());
  ball = engine->getNodeList(BallMotionNode().typeId());
  fence= engine->getNodeList(BricksNode().typeId());
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
  auto pad= CC_GNLF(Paddle,paddle,"paddle");
  auto ba= CC_GNLF(Ball,ball,"ball");
  auto pos= ba->pos();

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
  auto pad= CC_GNLF(Paddle,paddle,"paddle");
  auto ba = CC_GNLF(Ball,ball,"ball");

  if (cx::collide(pad,ba)) {
    check();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::check() {
  auto pad= CC_GNLF(Paddle,paddle,"paddle");
  auto ba= CC_GNLF(Ball,ball,"ball");
  auto sz= ba->sprite->getContentSize();
  auto hh= sz.height * 0.5f;
  auto pos= ba->pos();
  auto top= cx::getTop(pad->sprite);

  ba->setPos(pos.x, top+hh);
  ba->vel.y = - ba->vel.y;
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::checkBricks() {
  auto fen= CC_GNLF(BrickFence,fence,"fence");
  auto ba = CC_GNLF(Ball,ball,"ball");
  auto &bss= fen->bricks;

  for (auto n=0; n < bss.size(); ++n) {
    if (! bss[n]->status) { continue; }
    if (cx::collide(ba, bss[n])) {
      onBrick(bss[n]);
      break;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::onBrick(Brick *brick) {
  auto bal= CC_GNLF(Ball,ball,"ball");
  auto bz = bal->sprite->getContentSize();
  auto kz= brick->sprite->getContentSize();
  auto ks= brick->sprite;
  auto bs= bal->sprite;
  auto ka= cx::bbox4(ks);
  auto ba = cx::bbox4(bs);

  // ball coming down from top?
  if (ba.top > ka.top &&  ka.top > ba.bottom) {
    bal->vel.y = - bal->vel.y;
  }
  else
  // ball coming from bottom?
  if (ba.top > ka.bottom &&  ka.bottom > ba.bottom) {
    bal->vel.y = - bal->vel.y;
  }
  else
  // ball coming from left?
  if (ka.left > ba.left && ba.right > ka.left) {
    bal->vel.x = - bal->vel.x;
  }
  else
  // ball coming from right?
  if (ka.right > ba.left && ba.right > ka.right) {
    bal->vel.x = - bal->vel.x;
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


