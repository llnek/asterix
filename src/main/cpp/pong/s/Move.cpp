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
#include "Move.h"
#include "n/GNodes.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
void Move::addToEngine(not_null<a::Engine*> e) {
  FauxPaddleNode f;
  PaddleNode p;
  BallNode b;
  ArenaNode a;

  fauxNode= e->getNodeList(f.typeId());
  paddleNode= e->getNodeList(p.typeId());
  ballNode= e->getNodeList(b.typeId());
  arenaNode= e->getNodeList(a.typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Move::update(float dt) {
  if (MGMS()->isLive()) {
    auto ps = CC_GNF(Players,arenaNode->head,"players");

    for (auto node= paddleNode->head; node; node=node->next) {
      doit(node, dt);
    }

    for (auto node= fauxNode->head; node; node=node->next) {
      auto p = CC_GNF(Paddle,node,"paddle");
      auto &y= ps->parr[p->pnum];
      if (y.category == CC_CSV(c::Integer, "BOT")) {
        moveRobot(node, ballNode->head, dt);
      }
      else
      if (y.category == CC_CSV(c::Integer, "NET")) {
        simuMove(node, ballNode->head, dt);
      }
    }

    processBall(ballNode->head, dt);
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::simuMove(a::Node *node, a::Node *bnode, float dt) {
  auto lastpos= CC_GNF(Position, node, "lastpos");
  auto paddle = CC_GNF(Paddle,node,"paddle");
  auto cfg = MGMS()->getLCfg()->getValue();
  auto world= MGMS()->getEnclosureBox();
  auto hw2 = cx::halfHW(paddle->sprite);
  auto pos = paddle->pos();
  auto delta= dt * JS_FLOAT(cfg["PADDLE+SPEED"]);
  f::MaybeFloat x;
  f::MaybeFloat y;

  if (lastpos->lastDir > 0) {
    if (cx::isPortrait()) {
      x = f::MaybeFloat(pos.x + delta);
    } else {
      y = f::MaybeFloat(pos.y + delta);
    }
  }
  else
  if (lastpos->lastDir < 0) {
    if (cx::isPortrait()) {
      x = f::MaybeFloat(pos.x - delta);
    } else {
      y = f::MaybeFloat(pos.y - delta);
    }
  }

  if (! x.isNone()) {
    paddle->setPos(x.get(), pos.y);
    clamp(paddle->sprite);
  }
  if (! y.isNone()) {
    paddle->setPos(pos.x, y.get());
    clamp(paddle->sprite);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
//TODO: better AI please
void Move::moveRobot(a::Node *node, a::Node *bnode, float dt) {
  auto pad= CC_GNF(Paddle,node, "paddle");
  auto ball = CC_GNF(Ball,bnode,"ball");
  auto cfg= MGMS()->getLCfg()->getValue();
  auto speed= JS_FLOAT(cfg["PADDLE+SPEED"]);
  auto bp= ball->pos();
  auto pos = pad->pos();
  f::MaybeFloat y;
  f::MaybeFloat x;

  if (cx::isPortrait()) {
    if (bp.x > pos.x) {
      if (ball->vel.x > 0) {
        x = f::MaybeFloat(pos.x + dt * speed);
      }
    } else {
      if (ball->vel.x < 0) {
        x = f::MaybeFloat(pos.x - dt * speed);
      }
    }
  } else {
    if (bp.y > pos.y) {
      if (ball->vel.y > 0) {
        y = f::MaybeFloat(pos.y + dt * speed);
      }
    } else {
      if (ball->vel.y < 0) {
        y = f::MaybeFloat(pos.y - dt * speed);
      }
    }
  }

  if (! x.isNone()) {
    pad->setPos(x.get(),pos.y);
    clamp(pad->sprite);
  }

  if (! y.isNone()) {
    pad->setPos(pos.x, y.get());
    clamp(pad->sprite);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::processBall(a::Node *node, float dt) {
  auto world= MGMS()->getEnclosureBox();
  auto b= CC_GNF(Ball, node, "ball");
  c::Vec2 outVel;
  c::Vec2 outPos;
  auto rc= cx::traceEnclosure(
      dt,world, cx::bbox4(b->sprite),
      b->vel, outPos, outVel);

  if (rc) {
    b->vel.x = outVel.x;
    b->vel.y = outVel.y;
  }

  b->setPos(outPos.x,outPos.y);
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::doit(a::Node *node, float dt) {
  auto last= CC_GNF(Position, node, "lastpos");
  auto p= CC_GNF(Paddle, node, "paddle");
  auto cfg= MGMS()->getLCfg()->getValue();
  auto s= JS_FLOAT(cfg["PADDLE+SPEED"]) * dt;
  auto ld = last->lastDir;
  auto lp = last->lastP;
  auto &cs = p->kcodes;
  auto pos= p->pos();
  float nv, x,y;

  if (MGML()->keyPoll(cs[1])) {
    if (cx::isPortrait()) {
      x = pos.x + s;
      y = pos.y;
    } else {
      y = pos.y + s;
      x= pos.x;
    }
    p->setPos(x,y);
  }

  if (MGML()->keyPoll(cs[0])) {
    if (cx::isPortrait()) {
      x = pos.x - s;
      y = pos.y;
    } else {
      y = pos.y - s;
      x= pos.x;
    }
    p->setPos(x,y);
  }

  clamp(p->sprite);

  if (! MGMS()->isOnline()) {return;}

  // below is really for wsock stuff
  if (cx::isPortrait()) {
    nv = p->pos().x;
  } else {
    nv = p->pos().y;
  }

  auto delta= abs(nv - lp);
  auto dir=0;
  if (delta >= 1) {
    if (nv > lp) {
      dir=1;
      // moving up or right
    } else if (nv < lp) {
      dir= -1;
    }
  }
  last->lastP=nv;
  if (ld != dir) {
    if (MGMS()->isOnline()) { notifyServer(node,dir); }
    last->lastDir=dir;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::notifyServer(a::Node *node, int direction) {
  auto ps= CC_GNF(Players,arenaNode->head,"players");
  auto p2= ps->parr[2].color;
  auto p1= ps->parr[1].color;
  auto ss= CC_GNF(Slots,arenaNode->head,"slots");
  auto pad = CC_GNF(Paddle, node, "paddle");
  auto cfg= MGMS()->getLCfg()->getValue();
  auto pnum= ss->pnum;
  auto vv = direction * JS_FLOAT(cfg["PADDLE+SPEED"]);
  auto pos = pad->pos();
  auto key= pnum==2 ? p2 : p1;
  auto body= j::json({
      {"pnum", pnum},
      {key, j::json({
          {"x", pos.x },
          {"y", pos.y },
          {"dir", direction },
          {"pv", vv } }) }
  });

  ws::netSend(MGMS()->wsock(),
      ws::MType::SESSION,
      ws::EType::PLAY_MOVE,
      body);
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::clamp(c::Sprite *sprite) {
  auto world = MGMS()->getEnclosureBox();
  auto hw2= cx::halfHW(sprite);
  auto bb4= cx::bbox4(sprite);
  f::MaybeFloat x;
  f::MaybeFloat y;

  if (cx::isPortrait()) {
    if (bb4.right > world.right) {
        x = f::MaybeFloat(world.right - hw2.width);
    }
    if (bb4.left < world.left) {
        x = f::MaybeFloat(world.left + hw2.width);
    }
  } else {
    if (bb4.top > world.top) {
        y = f::MaybeFloat(world.top - hw2.height);
    }
    if (bb4.bottom < world.bottom) {
        y = f::MaybeFloat(world.bottom + hw2.height);
    }
  }

  if (!x.isNone()) {
    auto pos= sprite->getPosition();
    sprite->setPosition(x.get(), pos.y);
  }

  if (!y.isNone()) {
    auto pos= sprite->getPosition();
    sprite->setPosition(pos.x, y.get());
  }
}


NS_END(pong)


