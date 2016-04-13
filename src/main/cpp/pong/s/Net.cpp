// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Net.h"

NS_ALIAS(ws, fusii::odin)
NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
void Net::preamble() {
  _arena= _engine->getNodes("n/Players")[0];
  _ball= _engine->getNodes("n/Ball")[0];
  _engine->getNodes("n/Paddle", _paddles);
  if (MGMS()->isLive() &&
      MGMS()->isOnline()) {
    onceOnly();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
bool Net::update(float dt) {
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Net::onceOnly() {
  auto ss= CC_GEC(GVars, _arena,"n/GVars");
  auto cfg= MGMS()->getLCfg()->getValue();
  auto w= MGMS()->getEnclosureBox();
  auto p2= CC_CSS("P2_COLOR");
  auto p1= CC_CSS("P1_COLOR");
  auto src= j::json({
      {"framespersec",CC_CSV(c::Integer,"FPS") },
      {"world", j::json({
          {"top", w.top },
          {"right", w.right },
          {"bottom", w.bottom },
          {"left", w.left }
          }) },
      {"syncMillis", CC_CSV(c::Float, "syncMillis")},
      {"paddle", j::json({
          {"height", ss->pz.height },
          {"width", ss->pz.width },
          {"speed", JS_FLOAT(cfg["PADDLE+SPEED"]) }
          }) },
      {"ball", j::json({
          {"height", ss->bz.height },
          {"width", ss->bz.width },
          {"x", ss->bp.x },
          {"y", ss->bp.y },
          {"speed", JS_FLOAT(cfg["BALL+SPEED"]) },
          }) },
      {p2, j::json({
          {"x", ss->p2p.x },
          {"y", ss->p2p.y }
          }) },
      {p1, j::json({
          {"x", ss->p1p.x },
          {"y", ss->p1p.y }
          }) },
      {"numpts", JS_INT(cfg["NUM+POINTS"]) }
  });

  CCLOG("reply to server: session started ok");
  auto wsock = MGMS()->wsock();
  wsock->cancelAll();
  ws::netSend(wsock,
      ws::MType::SESSION,
      ws::EType::STARTED, src);
  wsock->listen([=](ws::OdinEvent *e) {
      this->onEvent(e);
    });
}

//////////////////////////////////////////////////////////////////////////////
//
void Net::onEvent(ws::OdinEvent *evt) {
  CCLOG("process: => %s", evt->getDoco().dump(2).c_str());
  switch (evt->getType()) {
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
  auto ss= CC_GEC(GVars, _arena,"n/GVars");
  j::json msg;
  switch (evt->getCode()) {
    case ws::EType::RESTART:
      CCLOG("restarting a new game...");
      SENDMSG("/game/restart");
    break;
    case ws::EType::STOP:
      CCLOG("game will stop");
      msg= evt->getDoco();
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
  auto ss= CC_GEC(GVars, _arena,"n/GVars");
  auto msg= evt->getDoco();
  if (!msg.is_object()) { return; }
  auto pnum= JS_INT(msg["pnum"]);
  switch (evt->getCode()) {
    case ws::EType::POKE_MOVE:
      CCLOG("activate arena, start to rumble!");
      if (ss->pnum == pnum) {
        ss->poked=true;
      } else {
        CCLOG("POKED with wrong player: %d", pnum);
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
  auto ps= CC_GEC(Players, _arena,"n/Players");
  auto p2=ps->parr[2].color;
  auto p1=ps->parr[1].color;
  auto rc = j::json({
      { p2, JS_INT(scores[p2]) },
      { p1, JS_INT(scores[p1]) }
  });
  SENDMSGEX("/hud/score/sync", &rc);
}

//////////////////////////////////////////////////////////////////////////////
//
void Net::process(ws::OdinEvent *evt) {
  auto ps= CC_GEC(Players, _arena,"n/Players");
  auto source = evt->getDoco()["source"];
  auto ok= true;

  if (source["winner"].is_object()) {
    auto pnum = JS_INT(source["winner"]["pnum"]);
    assert(pnum > 0 && pnum < ps->parr.size());
    auto &win= ps->parr[pnum];
    CCLOG("server sent us new winner ==> %s", win.color.c_str());
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
    reposNodes();
    ok=false;
    //ss->poked=false;
  }

  if (source["ball"].is_object()) {
    auto bm=CC_GEC(f::CMove,_ball,"f/CMove");
    auto ba= CC_GEC(Ball, _ball,"n/Ball");
    auto c = source["ball"];
    CCLOG("server says: Ball got SYNC'ED !!!");
    CC_POS2(ba, JS_FLOAT(c["x"]), JS_FLOAT(c["y"]));
    bm->vel.y= JS_FLOAT(c["vy"]);
    bm->vel.x= JS_FLOAT(c["vx"]);
  }

  syncPaddles(evt);
}

//////////////////////////////////////////////////////////////////////////////
//
void Net::reposPaddles() {
  auto ss= CC_GEC(GVars, _arena,"n/GVars");
  F__LOOP(it, _paddles) {
    auto &e= *it;
    auto last= CC_GEC(Position, e,"n/Position");
    auto p= CC_GEC(Paddle, e,"n/Paddle");
    if (p->pnum == 2) {
      CC_POS2(p, ss->p2p.x, ss->p2p.y);
      last->lastDir=0;
    }
    else
    if (p->pnum == 1) {
      CC_POS2(p, ss->p1p.x, ss->p1p.y);
      last->lastDir=0;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Net::reposNodes() {
  auto m= CC_GEC(f::CMove, _ball,"f/CMove");
  auto ss= CC_GEC(GVars, _arena,"n/GVars");
  auto b= CC_GEC(Ball, _ball,"n/Ball");

  reposPaddles();

  CC_POS2(b, ss->bp.x, ss->bp.y);
  m->vel.y=0;
  m->vel.x=0;
}

//////////////////////////////////////////////////////////////////////////////
//
void Net::syncPaddles(ws::OdinEvent *evt) {
  auto ps= CC_GEC(Players, _arena,"n/Players");
  auto source = evt->getDoco()["source"];
  auto p2= ps->parr[2].color;
  auto p1= ps->parr[1].color;
  F__LOOP(it, _paddles) {
    auto &e= *it;
    auto p= CC_GEC(Paddle, e,"n/Paddle");

    if (source[p2].is_object() &&
        p->pnum == 2) {
      CCLOG("server says: P2 got SYNC'ED !!!");
      syncOnePaddle(e, source[p2]);
    }

    if (source[p1].is_object() &&
        p->pnum == 1) {
      CCLOG("server says: P1 got SYNC'ED !!!");
      syncOnePaddle(e, source[p1]);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Net::syncOnePaddle(ecs::Node *node, j::json c) {
  auto last= CC_GEC(Position,node,"n/Position");
  auto p= CC_GEC(Paddle,node,"n/Paddle");
  auto dir=0;

  CC_POS2(p, JS_FLOAT(c["x"]), JS_FLOAT(c["y"]));
  if (JS_FLOAT(c["pv"]) > 0) { dir = 1;}
  if (JS_FLOAT(c["pv"]) < 0) { dir = -1;}
  last->lastDir = dir;
}



NS_END


