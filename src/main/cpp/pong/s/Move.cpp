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
#include "Move.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
void Move::preamble() {
  _arena= _engine->getNodes("n/Players")[0];
  _ball= _engine->getNodes("n/Ball")[0];
  _engine->getNodes("n/Paddle", _paddles);
}

//////////////////////////////////////////////////////////////////////////////
//
bool Move::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::process(float dt) {
  auto ps = CC_GEC(Players, _arena,"n/Players");

  F__LOOP(it, _paddles) {
    auto e= *it;
    auto p = CC_GEC(Paddle,e,"n/Paddle");
    auto f = CC_GEC(Faux,e,"n/Faux");
    auto &y= ps->parr[p->pnum];

    if (!f) {
      doit(e,dt);
    }
    else
    if (y.category == CC_CSV(c::Integer, "BOT")) {
      moveRobot(e, dt);
    }
    else
    if (y.category == CC_CSV(c::Integer, "NET")) {
      simuMove(e, dt);
    }
  }

  processBall(dt);
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::simuMove(ecs::Node *node, float dt) {
  auto lastpos= CC_GEC(Position, node, "n/Position");
  auto pad = CC_GEC(Paddle,node,"n/Paddle");
  auto cfg = MGMS()->getLCfg()->getValue();
  auto delta= dt * JS_FLOAT(cfg["PADDLE+SPEED"]);
  auto world= MGMS()->getEnclosureBox();
  auto hw2 = cx::halfHW(pad->node);
  auto pos = pad->pos();
  f::MaybeFloat x;
  f::MaybeFloat y;

  if (lastpos->lastDir > 0) {
    x = f::MaybeFloat(pos.x + delta);
  }
  else
  if (lastpos->lastDir < 0) {
    x = f::MaybeFloat(pos.x - delta);
  }

  if (! x.isNone()) {
    pad->setPos(x.get(), pos.y);
    clamp(pad->node);
  }
  if (! y.isNone()) {
    pad->setPos(pos.x, y.get());
    clamp(pad->node);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
//TODO: better AI please
void Move::moveRobot(ecs::Node *node, float dt) {
  auto pad= CC_GEC(Paddle,node, "n/Paddle");
  auto ba= CC_GEC(Ball, _ball,"n/Ball");
    auto bm=CC_GEC(f::CMove, _ball, "f/CMove");
  auto cfg= MGMS()->getLCfg()->getValue();
  auto speed= JS_FLOAT(cfg["PADDLE+SPEED"]);
  auto pos = pad->pos();
  auto bp= ba->pos();
  f::MaybeFloat y;
  f::MaybeFloat x;

  if (bp.x > pos.x) {
    if (bm->vel.x > 0) {
      x = f::MaybeFloat(pos.x + dt * speed);
    }
  } else {
    if (bm->vel.x < 0) {
      x = f::MaybeFloat(pos.x - dt * speed);
    }
  }

  if (! x.isNone()) {
    pad->setPos(x.get(),pos.y);
    clamp(pad->node);
  }

  if (! y.isNone()) {
    pad->setPos(pos.x, y.get());
    clamp(pad->node);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::processBall(float dt) {
  auto m= CC_GEC(f::CMove, _ball, "f/CMove");
  auto b= CC_GEC(Ball, _ball, "n/Ball");
  auto world= MGMS()->getEnclosureBox();
  c::Vec2 outPos;
  c::Vec2 outVel;
  auto rc= cx::traceEnclosure(
      dt,world, cx::bbox4(b->node),
      m->vel, outPos, outVel);

  m->vel.x = outVel.x;
  m->vel.y = outVel.y;
  b->setPos(outPos.x,outPos.y);
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::doit(ecs::Node *node, float dt) {
  auto last= CC_GEC(Position, node, "n/Position");
  auto p= CC_GEC(Paddle, node, "n/Paddle");
  auto cfg= MGMS()->getLCfg()->getValue();
  auto s= JS_FLOAT(cfg["PADDLE+SPEED"]) * dt;
  auto ld = last->lastDir;
  auto lp = last->lastP;
  auto &cs = p->kcodes;
  auto pos= p->pos();
  float nv, x,y;

  if (MGML()->keyPoll(cs[1])) {
    x = pos.x + s;
    y = pos.y;
    p->setPos(x,y);
  }

  if (MGML()->keyPoll(cs[0])) {
    x = pos.x - s;
    y = pos.y;
    p->setPos(x,y);
  }

  clamp(p->node);

  // below is really for wsock stuff
  if (! MGMS()->isOnline()) {
    return;
  }

  nv = p->pos().x;

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
    notifyServer(node,dir);
    last->lastDir=dir;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::notifyServer(ecs::Node *node, int dir) {
  auto ps= CC_GEC(Players, _arena,"n/Players");
  auto ss= CC_GEC(GVars, _arena,"n/GVars");
  auto pad = CC_GEC(Paddle, node, "n/Paddle");
  auto p2= ps->parr[2].color;
  auto p1= ps->parr[1].color;
  auto cfg= MGMS()->getLCfg()->getValue();
  auto pnum= ss->pnum;
  auto vv = dir * JS_FLOAT(cfg["PADDLE+SPEED"]);
  auto pos = pad->pos();
  auto key= pnum==2 ? p2 : p1;
  auto body= j::json({
      {"pnum", pnum},
      {key, j::json({
          {"x", pos.x },
          {"y", pos.y },
          {"dir", dir},
          {"pv", vv } }) }
  });

  ws::netSend(
      MGMS()->wsock(),
      ws::MType::SESSION,
      ws::EType::PLAY_MOVE,
      body);
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::clamp(c::Node *sprite) {
  auto world = MGMS()->getEnclosureBox();
  auto hw2= cx::halfHW(sprite);
  auto bb4= cx::bbox4(sprite);
  f::MaybeFloat x;
  f::MaybeFloat y;

  if (bb4.right > world.right) {
    x = f::MaybeFloat(world.right - hw2.width);
  }
  if (bb4.left < world.left) {
    x = f::MaybeFloat(world.left + hw2.width);
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


NS_END


