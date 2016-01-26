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
  faux= engine->getNodeList(FauxPaddleNode().typeId());
  arena = engine->getNodeList(ArenaNode().typeId());
  paddle = engine->getNodeList(PaddleNode().typeId());
  ball = engine->getNodeList(BallNode().typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Resolve::update(float dt) {

  if (!MGMS()->isOnline() &&
      MGMS()->isLive()) {

    int win= checkNodes(paddle);
    if (win > 0) {
      onWin(win);
    }
    else {
      win= checkNodes(faux);
      if (win > 0) {
        onWin(win);
      }
    }
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
int Resolve::checkNodes(a::NodeList *nl) {
  for (auto node=nl->head; node; node=node->next) {
    auto winner = check(node);
    if (winner > 0) {
      return winner;
    }
  }
  return 0;
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::onWin(int winner) {
  auto ps= CC_GNLF(Players,arena, "players");
  auto ss= CC_GNLF(GVars,arena,"slots");
  auto ba= CC_GNLF(Ball, ball, "ball");
  auto cfg= MGMS()->getLCfg()->getValue();
  auto speed= JS_FLOAT(cfg["BALL+SPEED"]);

  assert(winner > 0 && winner < ps->parr.size());
  CCLOG("winner ====== %d", winner);

  ba->vel.x = speed * cx::randSign();
  ba->vel.y = speed * cx::randSign();
  ba->setPos( ss->bp.x, ss->bp.y);

  auto msg= j::json({
        {"score", 1},
        {"pnum", winner},
        {"color", ps->parr[winner].color }
  });

  SENDMSGEX("/hud/score/update", &msg);
}

//////////////////////////////////////////////////////////////////////////////
//
int Resolve::check(a::Node *node) {
  auto pd= CC_GNF(Paddle, node, "paddle");
  auto b= CC_GNLF(Ball, ball, "ball");
  auto pc= pd->pnum;
  auto bp= b->pos();

  if (pc == 1) {
    return bp.y < cx::getBottom(pd->sprite) ? 2 : 0;
  }

  if (pc == 2) {
    return bp.y > cx::getTop(pd->sprite) ? 1 : 0;
  }

}

NS_END(pong)


