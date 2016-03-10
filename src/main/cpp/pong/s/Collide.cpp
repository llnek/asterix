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
NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
void Collide::preamble() {
  arena = engine->getEntities("n/Players")[0];
  engine->getEntities("n/Paddle",paddles);
  ball = engine->getEntities("n/Ball")[0];
}

//////////////////////////////////////////////////////////////////////////////
//
bool Collide::update(float dt) {

  if (MGMS()->isLive()) {
    process(dt);
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::process(float dt) {
  auto ba = CC_GEC(Ball, ball, "n/Ball");
  F__LOOP(it, paddles) {
    auto e= *it;
    auto p= CC_GEC(Paddle,e, "n/Paddle");
    if (cx::collide(p, ba)) {
      check(p, ba);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::check(Paddle *pad, Ball *ball) {
  auto hw2= cx::halfHW(ball->node);
  auto bb4 = cx::bbox4(pad->node);
  auto pos = ball->pos();
  auto x= pos.x;
  auto y= pos.y;

  ball->vel.y = - ball->vel.y;

  if (pad->pnum == 1) {
    y=bb4.top + hw2.height + 1;
  }

  if (pad->pnum == 2) {
    y = bb4.bottom - hw2.height - 1;
  }

  ball->setPos(x,y);
  cx::sfxPlay(pad->snd);
}


NS_END


