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
#include "Net.h"

NS_ALIAS(ws, fusii::odin)
NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
void Net::addToEngine(not_null<a::Engine*> e) {
  FauxPaddleNode f;
  PaddleNode p;
  BallNode b;
  AreanNode a;
  paddleNode= e->getNodeList(p.typeId());
  ballNode= e->getNodeList(b.typeId());
  fauxNode= e->getNodeList(f.typeId());
  arenaNode= e->getNodeList(a.typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Net::update(float dt) {
  if (MGMS()->isLive() &&
      MGMS()->isOnline()) {
    if (! inited) {
      onceOnly();
    } else {
      doit(dt);
    }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Net::doit(float dt) {
  const evt = this.evQ.length > 0 ? this.evQ.shift() : undef;
  if (!!evt) {
    this.onevent(evt.event);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Net::onceOnly() {
  CCLOG("reply to server: session started ok");
  auto ss= CC_GNF(Slots,arenaNode->head,"slots");
  auto cfg= MGMS()->getLCfg()->getValue();
  auto wsock = MGMS()->wsock();
  auto src= j::json({
      {"framespersec",CC_CSV(c::Integer,"FPS") },
      {"world", j::json({}) },
      {"syncMillis", CC_CSV(c::Float, "syncMillis")},
      {"paddle", j::json({
          {"height", floor(ss->pz.height) },
          {"width", floor(ss->pz.width) },
          {"speed", floor(JS_FLOAT(cfg["PADDLE+SPEED"])) }
          }) },
      {"ball", j::json({
          {"height", floor(ss->bz.height) },
          {"width", floor(ss->bz.width) },
          {"x", floor(ss->bp.x) },
          {"y", floor(ss->bp.y) },
          {"speed", floor(JS_FLOAT(cfg["BALL+SPEED"])) },
          }) },
      {"p2", j::json({
          {"x", floor(ss->p2p.x)},
          {"y", floor(ss->p2p.y) }
          }) },
      {"p1", j::json({
          {"x", floor(ss->p1p.x)},
          {"y", floor(ss->p1p.y)}
          }) },
      {"numpts", JS_INT(cfg["NUM+POINTS"]) }
  });

  wsock->cancenlAll();
  ws::netsend( ws::MType::SESSION, ws::EType::STARTED, src);
  wsock->listen([=](ws::OdinEvent *e) {
      this->onEvent(e);
    });
  inited=true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Net::onEvent(ws::OdinEvent *evt) {
  switch (evt->type) {
    case ws::MType::NETWORK:
      onnetw(evt);
    break;
    case ws::MType::SESSION:
      onsess(evt);
    break;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Net::onnetw(ws::OdinEvent *evt) {
  auto ss= CC_GNF(Slots,arenaNode->head,"slots");
  j::json msg;
  switch (evt->code) {
    case ws::EType::RESTART:
      CCLOG("restarting a new game...");
      SENDMSG("/game/restart");
    break;
    case ws::EType::STOP:
      CCLOG("game will stop");
      msg= evt->doco;
      SENDMSGEX("/game/stop", &msg);
    break;
    case ws::EType::SYNC_ARENA:
      CCLOG("synchronize ui as defined by server");
      process(evt);
      ss->poked=true;
    break;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Net::onsess(ws::OdinEvent *evt) {
  auto ss= CC_GNF(Slots,arenaNode->head,"slots");
  auto msg= evt->doco;
  if (!msg.is_object()) { return; }
  switch (evt->code) {
    case ws::EType::POKE_MOVE:
      CCLOG("activate arena, start to rumble!");
      if (ss->pnum == JS_INT(msg["pnum"])) {
        ss->poked=true;
      } else {
        CCLOG("POKED with wrong player: %d", JS_INT(msg["pnum"]));
      }
    break;
    case ws::EType::SYNC_ARENA:
      CCLOG("synchronize ui as defined by server.");
      process(evt);
      ss->poked=true;
    break;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Net::syncScores(j::json scores) {
  auto ps= CC_GNF(Players,arenaNode,"players");
  auto rc = j::json({
      { ps->parr[2].color, JS_INT(scores["p2"]) },
      { ps->parr[1].color, js_INT(scores["p1"]) }
  });
  SENDMSGEX("/hud/score/sync", &rc);
}

//////////////////////////////////////////////////////////////////////////////
//
void Net::process(ws::OdinEvent *evt) {
  CCLOG("process: => %s", evt->doco.dump().c_str());
  auto ps= CC_GNF(Players,arenaNode,"players");
  auto source = evt->doco["source"];
  auto ok= true;

  if (source["winner"].is_object()) {
    auto pnum = JS_INT(source["winner"]["pnum"]);
    assert(pnum > 0 && pnum < ps->parr.size());
    auto &win= ps->parr[pnum];
    CCLOG("server sent us new winner ==> %s", win.color);
    syncScores(source["winner"]["scores"]);
    auto msg= j::json({
        {"winner", win.color},
        {"pnum", win.pnum}
        });
    SENDMSGEX("/hud/end", &msg);
  }

  if (source["scores"].is_object()) {
    CCLOG("server sent us new scores !!!!");
    syncScores(source["scores"]);
    // once we get a new score, we reposition the entities
    // and pause the game (no moves) until the server
    // tells us to begin a new point.
    reposEntities();
    ok=false;
    //ss->poked=false;
  }

  if (source["ball"].is_object()) {
    auto ball= CC_GNF(Ball,ballNode->head,"ball");
    auto c = source["ball"];
    CCLOG("server says: Ball got SYNC'ED !!!");
    ball->setPos(JS_FLOAT(c["x"]), JS_FLOAT(c["y"]));
    ball->vel.y= JS_FLOAT(c["vy"]);
    ball->vel.x= JS_FLOAT(c["vx"]);
  }

  syncPaddles(paddleNode, evt);
  syncPaddles(fauxNode, evt);

  return ok;
}

//////////////////////////////////////////////////////////////////////////////
//
void Net::reposPaddles(a::NodeList *nl) {
  auto ss= CC_GNF(Slots,arenaNode-head,"slots");
  for (auto node=nl->head; node; node=node->next) {
    auto last= CC_GNF(Position,node,"lastpos");
    auto p= CC_GNF(Paddle,node,"paddle");
    if (p->pnum == 2) {
      p->setPos(ss->p2p.x, ss->p2p.y);
      last->lastDir=0;
    }
    else
    if (p->pnum == 1) {
      p->setPos(ss->p1p.x, ss->p1p.y);
      last->lastDir=0;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Net::reposEntities() {
  auto ss= CC_GNF(Slots,arenaNode->head,"slots");
  auto ball= CC_GNF(Ball,ballNode->head,"ball");

  reposPaddles(paddleNode);
  reposPaddles(fauxNode);

  ball->setPos(ss->bp.x, ss->bp.y);
  ball->vel.y=0;
  ball->vel.x=0;
}

//////////////////////////////////////////////////////////////////////////////
//
void Net::syncPaddles(a::NodeList *nl, ws::OdinEvent *evt) {
  auto source = evt->doco["source"];
  for (auto node = nl->head; node; node=node->next) {
    auto p= CC_GNF(Paddle,node,"paddle");

    if (source["p2"].is_object() &&
        p->pnum == 2) {
      CCLOG("server says: P2 got SYNC'ED !!!");
      syncOnePaddle(node, source["p2"]);
    }

    if (source["p1"].is_object() &&
        p->pnum == 1) {
      CCLOG("server says: P1 got SYNC'ED !!!");
      syncOnePaddle(node, source["p1"]);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Net::syncOnePaddle(a::Node *node, j::json c) {
  auto last= CC_GNF(Position,node,"lastpos");
  auto p= CC_GNF(Paddle,node,"paddle");
  auto dir=0;

  p->setPos(JS_FLOAT(c["x"]), JS_FLOAT(c["y"]));
  if (JS_FLOAT(c["pv"]) > 0) { dir = 1;}
  if (JS_FLOAT(c["pv"]) < 0) { dir = -1;}
  last->lastDir = dir;
}



NS_END(pong)


