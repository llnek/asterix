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
Resolve::Resolve(not_null<EFactory*> f, not_null<c::Dictionary*> o)
  : XSystem<EFactory>(f,o) {
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::addToEngine(not_null<a::Engine*> e) {
  FauxPaddleNode f;
  fauxs= e->getNodeList(f.typeId());
  PaddleNode p;
  nodeList= e->getNodeList(p.typeId());
  BallNode b;
  balls= e->getNodeList(b.typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Resolve::update(float dt) {
  auto bnode = balls->head;

  if (MGMS()->isOnline()) { return; }

  if (MGMS()->isLive()) {

    rc= checkNodes(nodeList, bnode);
    if (rc) {
      rc= checkNodes(fauxs, bnode);
    }
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
bool Resolve::checkNodes(a::NodeList *nl, a::Node *bnode) {
  for (auto node=nl->head; node; node=node->next) {
    auto winner = check(node,bnode);
    if (winner.length() > 0) {
      onWin(winner);
      return false;
    }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::onWin(const sstr &winner) {
  auto velo= CC_GNF(Velocity, balls->head, "velocity");
  auto ball= CC_GNF(Ball, balls->head, "ball");
  CCLOG("winner ====== %s", winner.c_str());
  ball->setPos( BALL.x, BALL.y);
  velo->vel.x = BALL.speed * cx::randSign();
  velo->vel.y = BALL.speed * cx::randSign();
  sendEx("/hud/score/update",
      j::json({
        { "score", 1},
        {"color", winner }
      }));
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


