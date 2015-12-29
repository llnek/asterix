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

    rc= checkNodes(paddleNode, ballNode->head);
    if (rc) {
      rc= checkNodes(fauxNode, ballNode->head);
    }
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
bool Resolve::checkNodes(a::NodeList *nl, a::Node *bnode) {
  for (auto node=nl->head; node; node=node->next) {
    auto winner = check(node,bnode);
    if (winner > 0) {
      onWin(winner);
      return false;
    }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::onWin(int winner) {
  auto ps= CC_GNF(Players,arenaNode->head, "players");
  auto ss= CC_GNF(Slots,arenaNode->head,"slots");
  auto ball= CC_GNF(Ball, balls->head, "ball");
  auto cfg= MGMS()->getLCfg()->getValue();
  auto speed= JS_FLOAT(cfg["BALL+SPEED"]);

  CCLOG("winner ====== %d", winner);
  ball->setPos( ss->bp.x, ss->bp.y);
  ball->vel.x = speed * cx::randSign();
  ball->vel.y = speed * cx::randSign();

  auto msg= j::json({
        {"score", 1},
        {"pnum", winner},
        {"color", ps->parr[pnum].color }
  });

  SENDMSGEX("/hud/score/update", &msg);
}

//////////////////////////////////////////////////////////////////////////////
//
const sstr Resolve::check(a::Node *node, a::Node *bnode) {
  auto pd= CC_GNF(Paddle, node, "paddle");
  auto b= CC_GNF(Ball, bnode, "ball");
  auto pc= pd->color;
  auto bp= b->getPos();

  if (cx::isPortrait()) {

    if (pc == CC_CSS("P1_COLOR")) {
      return bp.y < cx::getBottom(pd->sprite) ?
        CC_CSS("P2_COLOR") : "";
    } else {
      return bp.y > cx::getTop(pd->sprite) ?
        CC_CSS("P1_COLOR") : "";
    }

  } else {

    if (pc == CC_CSS("P1_COLOR")) {
      return bp.x < cx::getLeft(pd->sprite) ?
        CC_CSS("P2_COLOR") : "";
    } else {
      return bp.x > cx::getRight(pd->sprite) ?
        CC_CSS("P1_COLOR") : "";
    }

  }
}

NS_END(pong)


