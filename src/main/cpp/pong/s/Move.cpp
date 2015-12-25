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

#include "Move.h"

NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
Move::Move(not_null<EFactory*> f, not_null<c::Dictionary*> o)
  : XSystem<EFactory>(f,o) {
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::addToEngine(not_null<a::Engine*> e) {
  FauxPaddleNode f;
  fauxs= e->getNodeList(f.typeId());
  PaddleNode p;
  paddles= e->getNodeList(p.typeId());
  BallNode b;
  balls= e->getNodeList(b.typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Move::update(float dt) {
  auto bnode= balls->head;

  if (MGMS()->isLive()) {

    for (auto node= paddles->head; node; node=node->next) {
      doit(node, dt);
    }

    for (auto node= fauxs->head; node; node=node->next) {
      auto p = CC_GNF(Player,node, "player");
      if (p->category == CC_CSV(c::Integer, "BOT")) {
        moveRobot(node, bnode, dt);
      }
      else
      if (p->category == CC_CSV(c::Integer, "NET")) {
        simuMove(node, bnode, dt);
      }
    }

    processBall(bnode, dt);
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::simuMove(a::Node *node, a::Node *bnode, float dt) {
  auto paddle = CC_GNF(Paddle,node,"paddle");
  auto world= MGMS()->getEnclosureBox();
  auto hw2 = cx::halfHW(paddle->sprite),
  auto pos = paddle->pos();
  auto lastpos= CC_GNF(Position, node, "lastpos");
  auto delta= dt * paddle->speed;
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
  auto velo = CC_GNF(Velocity,bnode,"velocity");
  auto ball = CC_GNF(Ball,bnode,"ball");
  auto pad= CC_GNF(Paddle,node, "paddle");
  auto bp= ball->getPos();
  auto pos = pad->getPos();
  auto speed= pad->speed;
  f::MaybeFloat y;
  f::MaybeFloat x;

  if (cx::isPortrait()) {
    if (bp.x > pos.x) {
      if (velo->vel.x > 0) {
        x = f::MaybeFloat(pos.x + dt * speed);
      }
    } else {
      if (velo->vel.x < 0) {
        x = f::MaybeFloat(pos.x - dt * speed);
      }
    }
  } else {
    if (bp.y > pos.y) {
      if (velo->vel.y > 0) {
        y = f::MaybeFloat(pos.y + dt * speed);
      }
    } else {
      if (velo->vel.y < 0) {
        y = f::MaybeFloat(pos.y - dt * speed);
      }
    }
  }

  if (! x.isNone()) {
    paddle->setPos(x.get(),pos.y);
    clamp(paddle->sprite);
  }

  if (! y.isNone()) {
    paddle->setPos(pos.x, y.get());
    clamp(paddle->sprite);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::processBall(a::Node *node, float dt) {
  auto v = CC_GNF(Velocity, node, "velocity");
  auto b= CC_GNF(Ball, node, "ball");
  auto pos= b->getPos();
  c::Rect rect( cx::bbox(b->sprite));

  rect.x = pos.x;
  rect.y = pos.y;

  c::Vec2 outVel;
  c::Vec2 outPos;
  auto rc= cx::traceEnclosure(dt,MGMS()->getEnclosureBox(),
                         rect,
                         v->vel, outPos, outVel);
  if (rc) {
    v->vel.x = outVel.x;
    v->vel.y = outVel.y;
  }
  b->setPos(outPos.x,outPos.y);
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::doit(a::Node *node, float dt) {
  auto last= CC_GNF(Position, node, "lastpos");
  auto p= CC_GNF(Paddle, node, "paddle");
  auto s= p->speed * dt;
  auto m= CC_GNF(Motion, node, "motion");
  auto ld = last->lastDir;
  auto lp = last->lastP;
  auto pos= p->getPos();
  float nv, x,y;

  if (m->right) {
    if (cx::isPortrait()) {
      x = pos.x + s;
      y = pos.y;
    } else {
      y = pos.y + s;
      x= pos.x;
    }
    p->setPos(x,y);
  }

  if (m->left) {
    if (cx::isPortrait()) {
      x = pos.x - s;
      y = pos.y;
    } else {
      y = pos.y - s;
      x= pos.x;
    }
    p->setPos(x,y);
  }

  m->right = false;
  m->left= false;

  clamp(p->sprite);

  // below is really for wsock stuff
  if (cx::isPortrait()) {
    nv = p->getPos().x;
  } else {
    nv = p->getPos().y;
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
  auto pnum= CC_GDV(c::Integer, this->state);
  auto pad = CC_GNF(Paddle, node, "paddle");
  auto vv = direction * pad->speed;
  auto pos = pad->getPos();
  auto key= pnum==2 ? "p2" : "p1";
  auto body= j::json({
      {"pnum", pnum},
      {key, j::json({
          {"x", floor(pos.x) },
          {"y", floor(pos.y) },
          {"dir", direction },
          {"pv", vv } }) }
  });

  ws::netsend(ws::MType::SESSION,
      ws::EType::PLAY_MOVE,
      body);
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::clamp(c::Sprite *sprite) {
  auto world = MGMS()->getEnclosureBox();
  auto pos= sprite->getPosition();
  auto hw2= cx::halfHW(sprite);
  auto bb4= cx::bbox4(sprite);
  f::MaybeFloat x;
  f::MaybeFloat y;

  if (cx::isPortrait()) {
    if (bb4.right > world.right) {
      x = world.right - hw2.x;
    }
    if (bb4.left < world.left) {
      x = world.left + hw2.x;
    }
  } else {
    if (bb4.top > world.top) {
      y = world.top - hw2.y;
    }
    if (bb4.bottom < world.bottom) {
      y = world.bottom + hw2.y;
    }
  }

  if (!x.isNpne()) {
    sprite->setPosition(x.get(), pos.y);
  }

  if (!y.isNone()) {
    sprite->setPosition(pos.x, y.get());
  }
}


NS_END(pong)


