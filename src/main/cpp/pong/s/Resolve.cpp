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
#include "Resolve.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::preamble() {
  _arena = _engine->getNodes("n/Players")[0];
  _ball = _engine->getNodes("n/Ball")[0];
  _engine->getNodes("n/Paddle", _paddles);
}

//////////////////////////////////////////////////////////////////////////////
//
bool Resolve::update(float dt) {

  if (!MGMS()->isOnline() &&
      MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::process(float dt) {
  int win= checkNodes();
  if (win > 0) {
    onWin(win);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
int Resolve::checkNodes() {
  F__LOOP(it, _paddles) {
    auto e= *it;
    auto winner = check(e);
    if (winner > 0) {
      return winner;
    }
  }
  return 0;
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::onWin(int winner) {
  auto ps= CC_GEC(Players, _arena, "n/Players");
  auto ss= CC_GEC(GVars, _arena,"n/GVars");
  auto ba= CC_GEC(Ball, _ball, "n/Ball");
  auto bm= CC_GEC(f::CMove, _ball, "f/CMove");
  auto cfg= MGMS()->getLCfg()->getValue();
  auto speed= JS_FLOAT(cfg["BALL+SPEED"]);

  assert(winner > 0 && winner < ps->parr.size());
  CCLOG("winner ====== %d", winner);

  bm->vel.x = speed * cx::randSign();
  bm->vel.y = speed * cx::randSign();
  ba->setPos(ss->bp.x, ss->bp.y);

  auto msg= j::json({
        {"score", 1},
        {"pnum", winner},
        {"color", ps->parr[winner].color }
  });

  SENDMSGEX("/hud/score/update", &msg);
}

//////////////////////////////////////////////////////////////////////////////
//
int Resolve::check(ecs::Node *node) {
  auto pd= CC_GEC(Paddle, node, "n/Paddle");
  auto b= CC_GEC(Ball, _ball, "n/Ball");
  auto pc= pd->pnum;
  auto bp= b->pos();

  if (pc == 1) {
    return bp.y < cx::getBottom(pd->node) ? 2 : 0;
  }

  if (pc == 2) {
    return bp.y > cx::getTop(pd->node) ? 1 : 0;
  }

}

NS_END


