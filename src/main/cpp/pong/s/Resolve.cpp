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
#include "n/GNodes.h"
#include "Resolve.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::addToEngine(not_null<a::Engine*> e) {
  FauxPaddleNode f;
  PaddleNode p;
  BallNode b;
  ArenaNode a;

  arenaNode = e->getNodeList(a.typeId());
  fauxNode= e->getNodeList(f.typeId());
  paddleNode = e->getNodeList(p.typeId());
  ballNode = e->getNodeList(b.typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Resolve::update(float dt) {

  if (!MGMS()->isOnline() &&
      MGMS()->isLive()) {

    int win= checkNodes(paddleNode, ballNode->head);
    if (win > 0) {
      onWin(win);
    } else {
      checkNodes(fauxNode, ballNode->head);
    }
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
int Resolve::checkNodes(a::NodeList *nl, a::Node *bnode) {
  for (auto node=nl->head; node; node=node->next) {
    auto winner = check(node,bnode);
    if (winner > 0) {
      return winner;
    }
  }
  return 0;
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::onWin(int winner) {
  auto ps= CC_GNF(Players,arenaNode->head, "players");
  auto ss= CC_GNF(Slots,arenaNode->head,"slots");
  auto ball= CC_GNF(Ball, ballNode->head, "ball");
  auto cfg= MGMS()->getLCfg()->getValue();
  auto speed= JS_FLOAT(cfg["BALL+SPEED"]);

  assert(winner > 0 && winner < ps->parr.size());
  CCLOG("winner ====== %d", winner);

  ball->vel.x = speed * cx::randSign();
  ball->vel.y = speed * cx::randSign();
  ball->setPos( ss->bp.x, ss->bp.y);

  auto msg= j::json({
        {"score", 1},
        {"pnum", winner},
        {"color", ps->parr[winner].color }
  });

  SENDMSGEX("/hud/score/update", &msg);
}

//////////////////////////////////////////////////////////////////////////////
//
int Resolve::check(a::Node *node, a::Node *bnode) {
  auto pd= CC_GNF(Paddle, node, "paddle");
  auto b= CC_GNF(Ball, bnode, "ball");
  auto pc= pd->pnum;
  auto bp= b->pos();

  if (cx::isPortrait()) {

    if (pc == 1) {
      return bp.y < cx::getBottom(pd->sprite) ? 2 : 0;
    }

    if (pc == 2) {
      return bp.y > cx::getTop(pd->sprite) ? 1 : 0;
    }

  } else {

    if (pc == 1) {
      return bp.x < cx::getLeft(pd->sprite) ? 2 : 0;
    }
    if (pc == 2) {
      return bp.x > cx::getRight(pd->sprite) ? 1 : 0;
    }

  }
}

NS_END(pong)


