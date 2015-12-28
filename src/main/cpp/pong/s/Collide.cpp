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

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Collide.h"

NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
Collide::Collide(not_null<EFactory*> f, not_null<c::Dictionary*> o)
: XSystem<EFactory>(f,o) {

}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::addToEngine(not_null<a::Engine*> e) {
  FauxPaddleNode f;
  fauxs= e->getNodeList(f.typeId());
  PaddleNode p;
  nodeList= e->getNodeList(p.typeId());
  BallNode b;
  balls= e->getNodeList(b.typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Collide::update(float dt) {
  auto bnode = balls->head;

  if (MGMS()->isLive()) {
    checkNodes(nodeList, bnode);
    checkNodes(fauxs, bnode);
  }
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
  /**
   * Ball hits paddle.
   * @method check
   * @private
   */
void Collide::check(a::Node *node, a::Node *bnode) {
  auto velo = CC_GNF(Velocity, bnode, "velocity");
  auto pad = CC_GNF(Paddle,node,"paddle");
  auto ball = CC_GNF(Ball,bnode,"ball");
  auto pos = ball->getPos();
  auto bb4 = cx::bbox4(pad->sprite);
  auto x= pos.x,
  auto y= pos.y,
  auto hw2= cx::halfHW(ball->sprite);

  if (cx::isPortrait()) {
    velo->vel.y = - velo->vel.y;
  } else {
    velo->vel.x = - velo->vel.x;
  }

  if (pad->color == CC_CSS("P1_COLOR")) {
    if (cx::isPortrait()) {
      y=bb4.top + hw2.y;
    } else {
      x=bb4.right + hw2.x;
    }
  } else {
    if (cx::isPortrait()) {
      y = bb4.bottom - hw2.y;
    } else {
      x = bb4.left - hw2.x;
    }
  }

  ball->setPos(x,y);
  cx::sfxPlay(pad->snd);
}



NS_END(pong)


