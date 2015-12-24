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
#include "Net.h"

NS_ALIAS(ws, fusii::odin)
NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
Net::Net(not_null<EFactory*> f, not_null<c::Dictionary*> o)
  : XSystem<EFactory>(f,o) {
}

//////////////////////////////////////////////////////////////////////////////
//
void Net::addToEngine(not_null<a::Engine*> e) {
  PaddleNode p;
  paddles= e->getNodeList(p.typeId());
  BallNode b;
  balls= e->getNodeList(b.typeId());
  FauxPaddleNode f;
  fauxs= e->getNodeList(f.typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Net::update(float dt) {
  if (MGMS()->isLive()) {
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
  if (MGMS()->isOnline()) {
    CCLOG("reply to server: session started ok");
    auto wsock = MGMS()->wsock();
    const src= R.pick(['framespersec',
                       'world',
                       'syncMillis',
                       'paddle',
                       'ball',
                       'p1',
                       'p2',
                       'numpts'], this.state);
    wsock->cancenlAll();
    ws::netsend(
        MType::SESSION,
        EType::STARTED, src);
    wsock->listen([=](ws::OdinEvent *e) {
        this->onEvent(e);
      });
  }
  inited=true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Net::onEvent(ws::OdinEvent *evt) {
  switch (evt->type) {
    case MType::NETWORK:
      onnetw(evt);
    break;
    case MType::SESSION:
      onsess(evt);
    break;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Net::onnetw(ws::OdinEvent *evt) {
  switch (evt->code) {
    case EType::RESTART:
      CCLOG("restarting a new game...");
      send("/game/restart");
    break;
    case EType::STOP:
      CCLOG("game will stop");
      sendEx("/game/stop", nullptr);//evt);
    break;
    case EType::SYNC_ARENA:
      CCLOG("synchronize ui as defined by server");
      process(evt);
      state->setObject(CC_BOOL(true), "poked");
    break;
  }
}

void Net::onsess(ws::OdinEvent *evt) {
  if (!sjs.isobj(evt.source)) { return; }
  switch (evt.code) {
    case evts.POKE_MOVE:
      sjs.loggr.debug("activate arena, start to rumble!");
      if (this.state.pnum === evt.source.pnum) {
        this.state.poked=true;
      } else {
        sjs.loggr.error("Got POKED but with wrong player number. " +
                        evt.source.pnum);
      }
    break;
    case evts.SYNC_ARENA:
      sjs.loggr.debug("synchronize ui as defined by server.");
      this.process(evt);
      this.state.poked=true;
    break;}
}

//////////////////////////////////////////////////////////////////////////////
//
void Net::syncScores(scores) {
  const actors= this.state.players,
  rc= {};
  rc[actors[2].color] = scores.p2;
  rc[actors[1].color] = scores.p1;
  sh.fire('/hud/score/sync', { points: rc});
}

//////////////////////////////////////////////////////////////////////////////
//
void Net::process(ws::OdinEvent *evt) {
  CCLOG("process: => %s", evt->doco.dump().c_str());
  auto source = evt->doco["source"];
  //let actors= this.state.players,
  auto ok= true;

  if (source["winner"].is_object()) {
    auto pnum = JS_INT(source["winner"]["pnum"]);
    auto &win= actors[pnum];
    CCLOG("server sent us new winner ==> %s", win->color);
    syncScores(source["winner"]["scores"]);
    sendEx("/hud/end",
        j::json({
          { "winner", win->color }
    }));
  }

  if (source["scores"].is_object()) {
    CCLOG("server sent us new scores !!!!");
    syncScores(source["scores"]);
    // once we get a new score, we reposition the entities
    // and pause the game (no moves) until the server
    // tells us to begin a new point.
    reposEntities();
    ok=false;
    //this.state.poked=false;
  }

  if (source["ball"].is_object()) {
    CCLOG("server says: Ball got SYNC'ED !!!");
    auto c = source["ball"];
    auto node= balls->head;
    auto &vel= node->velocity;
    auto &ball = node->ball;
    ball->sprite->setPosition(JS_FLOAT(c["x"]), JS_FLOAT(c["y"]));
    vel->vel.y= JS_FLOAT(c["vy"]);
    vel->vel.x= JS_FLOAT(c["vx"]);
  }

  syncPaddles(paddles, evt);
  syncPaddles(fauxs, evt);

  return ok;
}

//////////////////////////////////////////////////////////////////////////////
//
void Net::reposPaddles(a::NodeList *nl) {
  for (auto node=nl->head; node; node=node->next) {
    if (node->player->pnum == 2) {
      node->paddle->sprite->setPosition(
        PLAYER2.x,
        PLAYER2.y);
      node->lastpos->lastDir=0;
    }
    if (node->player->pnum == 1) {
      node->paddle->sprite->setPosition(
        PLAYER1.x,
        PLAYER1.y);
      node->lastpos->lastDir=0;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Net::reposEntities() {
  auto node=balls->head;

  reposPaddles(paddles);
  reposPaddles(fauxs);

  node->ball->sprite->setPosition(
    BALL.x,
    BALL.y);
  node->velocity->vel.y=0;
  node->velocity->vel.x=0;
}

//////////////////////////////////////////////////////////////////////////////
//
void Net::syncPaddles(a::NodeList *nl, ws::OdinEvent *evt) {
  auto source = evt->doco["source"];
  for (auto node = nl->head; node; node=node->next) {

    if (source["p2"].is_object() &&
        node->player->pnum == 2) {
      CCLOG("server says: P2 got SYNC'ED !!!");
      syncOnePaddle(node, source["p2"]);
    }

    if (source["p1"].is_object() &&
        node->player->pnum == 1) {
      CCLOG("server says: P1 got SYNC'ED !!!");
      syncOnePaddle(node, source["p1"]);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Net::syncOnePaddle(node, j::json c) {
  auto dir=0;
  node->paddle->sprite->setPosition(JS_INT(c["x"]), JS_INT(c["y"]));
  if (JS_INT(c["pv"] > 0) { dir = 1;}
  if (JS_INT(c["pv"] < 0) { dir = -1;}
  node->lastpos->lastDir = dir;
}

NS_END(pong)


