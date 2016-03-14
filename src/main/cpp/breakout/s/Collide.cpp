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
#include "Collide.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(breakout)

//////////////////////////////////////////////////////////////////////////////
//
void Collide::preamble() {
  _paddle = _engine->getNodes("f/CGesture")[0];
  _fence= _engine->getNodes("n/BrickFence")[0];
  _ball = _engine->getNodes("f/CAutoma")[0];
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
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
bool Collide::onPlayerKilled() {
  auto pad= CC_GEC(f::CPixie, _paddle, "f/CPixie");
  auto ba= CC_GEC(f::CPixie, _ball, "f/CPixie");
  auto pos= ba->pos();

  if (pos.y < cx::getBottom(pad->node)) {
    SENDMSG("/game/players/killed");
    return true;
  } else {
    return false;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::checkNodes() {
  auto pad= CC_GEC(f::CPixie, _paddle, "f/CPixie");
  auto ba = CC_GEC(f::CPixie, _ball, "f/CPixie");

  if (cx::collide(pad,ba)) {
    check();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::check() {
  auto pad= CC_GEC(f::CPixie, _paddle, "f/CPixie");
  auto ba= CC_GEC(f::CPixie, _ball, "f/CPixie");
  auto mv= CC_GEC(f::CMove, _ball, "f/CMove");
  auto sz= ba->csize();
  auto hh= HHZ(sz);
  auto pos= ba->pos();
  auto top= cx::getTop(pad->node);

  ba->setPos(pos.x, top+hh);
  mv->vel.y = - mv->vel.y;
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::checkBricks() {
  auto fen= CC_GEC(BrickFence, _fence,"n/BrickFence");
  auto ba = CC_GEC(f::CPixie, _ball, "f/CPixie");
  auto &bss= fen->bricks;

  F__LOOP(it,bss) {
    auto e= *it;
    if (!e->flipped &&
        cx::collide(ba, e)) {
      onBrick(e);
      break;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::onBrick(Brick *brick) {
  auto bs= CC_GEC(f::CPixie, _ball, "f/CPixie");
  auto mv= CC_GEC(f::CMove, _ball, "f/CMove");
  auto ka= cx::bbox4(brick);
  auto ba= cx::bbox4(bs);

  // ball coming down from top?
  if (ba.top > ka.top &&
      ka.top > ba.bottom) {
    mv->vel.y = - mv->vel.y;
  }
  else
  // ball coming from bottom?
  if (ba.top > ka.bottom &&
      ka.bottom > ba.bottom) {
    mv->vel.y = - mv->vel.y;
  }
  else
  // ball coming from left?
  if (ka.left > ba.left &&
      ba.right > ka.left) {
    mv->vel.x = - mv->vel.x;
  }
  else
  // ball coming from right?
  if (ka.right > ba.left &&
      ba.right > ka.right) {
    mv->vel.x = - mv->vel.x;
  }
  else {
    throw "Can't find the hit of ball & brick";
  }

  auto msg= j::json({
      {"value", brick->value }
      });
  SENDMSGEX("/game/players/earnscore", &msg);
  brick->flipped=true;
  brick->deflate();
}


NS_END


