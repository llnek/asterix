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
NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
void Collide::addToEngine(not_null<a::Engine*> e) {
  FauxPaddleNode f;
  PaddleNode p;
  BallNode b;
  ArenaNode a;

  arenaNode = e->getNodeList(a.typeId());
  fauxNode = e->getNodeList(f.typeId());
  paddleNode = e->getNodeList(p.typeId());
  ballNode = e->getNodeList(b.typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Collide::update(float dt) {
  if (MGMS()->isLive()) {
    checkNodes(paddleNode, ballNode->head);
    checkNodes(fauxNode, ballNode->head);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::checkNodes(a::NodeList *nl, a::Node *bnode) {
  auto ball = CC_GNF(Ball,bnode,"ball");
  for (auto node=nl->head; node; node=node->next) {
    if (cx::collide(CC_GNF(Paddle,node, "paddle"), ball)) {
      check(node, bnode);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::check(a::Node *node, a::Node *bnode) {
  auto pad = CC_GNF(Paddle,node,"paddle");
  auto ball = CC_GNF(Ball,bnode,"ball");
  auto pos = ball->pos();
  auto bb4 = cx::bbox4(pad->sprite);
  auto x= pos.x;
  auto y= pos.y;
  auto hw2= cx::halfHW(ball->sprite);

  if (cx::isPortrait()) {
    ball->vel.y = - ball->vel.y;
  } else {
    ball->vel.x = - ball->vel.x;
  }

  if (pad->pnum == 1) {
    if (cx::isPortrait()) {
      y=bb4.top + hw2.height;
    } else {
      x=bb4.right + hw2.width;
    }
  }

  if (pad->pnum == 2) {
    if (cx::isPortrait()) {
      y = bb4.bottom - hw2.height;
    } else {
      x = bb4.left - hw2.width;
    }
  }

  ball->setPos(x,y);
  cx::sfxPlay(pad->snd);
}


NS_END(pong)


