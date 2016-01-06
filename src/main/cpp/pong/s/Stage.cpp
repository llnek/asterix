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
#include "Stage.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
void Stage::preamble() {
  FauxPaddleNode f;
  PaddleNode p;
  BallNode b;
  ArenaNode a;

  fauxNode = engine->getNodeList(f.typeId());
  ballNode=engine->getNodeList(b.typeId());
  arenaNode = engine->getNodeList(a.typeId());
  paddleNode=engine->getNodeList(p.typeId());

  onceOnly();
}

//////////////////////////////////////////////////////////////////////////////
//
bool Stage::update(float dt) {
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Stage::onceOnly() {
  auto ss= CC_GNLF(GVars, arenaNode, "slots");
  auto world = MGMS()->getEnclosureBox();
  auto ps= cx::calcSize("red_paddle.png");
  auto bs= cx::calcSize("pongball.png");
  auto cw= cx::center();

  // position of paddles
  // portrait
  auto p1y = world.top * 0.1f + bs.height + HHZ(ps);
  auto p2y = world.top * 0.9f - bs.height - HHZ(ps);
  // landscape
  auto p2x = world.right - bs.width - HWZ(ps);
  auto p1x = world.left + bs.width + HWZ(ps);

  ss->pz= c::Size( ps.width, ps.height);
  ss->bz= c::Size( bs.width, bs.height);
  ss->bp= c::Vec2( cw.x, cw.y);

  if (cx::isPortrait()) {
    ss->p1p= c::Vec2(cw.x, p1y);
    ss->p2p= c::Vec2(cw.x, p2y);
  } else {
    ss->p1p= c::Vec2(p1x, cw.y);
    ss->p2p= c::Vec2(p2x, cw.y);
  }

  auto ball=CC_GNLF(Ball,ballNode, "ball");
  ball->inflate(ss->bp.x, ss->bp.y);

  initPaddles(paddleNode);
  initPaddles(fauxNode);
}

//////////////////////////////////////////////////////////////////////////////
//
void Stage::initPaddles(a::NodeList *nl) {
  auto ss= CC_GNLF(GVars,arenaNode,"slots");
   for (auto node=nl->head;node;node=node->next) {
    auto p= CC_GNF(Paddle,node,"paddle");
    if (p->pnum == 2) { p->inflate(ss->p2p.x, ss->p2p.y); }
    if (p->pnum == 1) { p->inflate(ss->p1p.x, ss->p1p.y); }
  }
}


NS_END(pong)


