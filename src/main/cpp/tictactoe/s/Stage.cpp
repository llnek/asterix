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
#include "core/Odin.h"
#include "ash/Node.h"
#include "n/CObjs.h"
#include "utils.h"
#include "Stage.h"

NS_ALIAS(ws, fusii::odin)
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tttoe)


//////////////////////////////////////////////////////////////////////////////
Stage::Stage(not_null<EFactory*> f, not_null<c::Dictionary*> d)
  : BaseSystem<EFactory>(f, d) {
}

//////////////////////////////////////////////////////////////////////////////
//
void Stage::addToEngine(not_null<a::Engine*> e) {

  CCLOG("adding system: Stage");

  BoardNode n;
  board = e->getNodeList(n.typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Stage::update(float dt) {

  CCLOG("Stage::update()");
  auto n = board->head;

  if (MGMS()->isLive()) {
    if (! inited) {
      onceOnly(n);
    }
    doIt(n);
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Stage::onceOnly(a::Node *node) {
  auto ps = CC_GNF(Players, node, "players");
  auto human = CC_CSV(c::Integer, "HUMAN");
  auto bot= CC_CSV(c::Integer,"BOT");
  auto pnum = 0;

  showGrid(node);

  if (MGMS()->isOnline()) {
    initOnline();
  } else {
    pnum= c::rand_0_1() > 0.5f ? 1 : 2;
    // randomly choose
    if (ps->parr[pnum].category == human) {
      MGMS()->sendMsg("/hud/timer/show");
    }
    else
    if (ps->parr[pnum].category == bot) {
      //noop
    }
  }

  state->setObject(CC_INT(pnum), "pnum");
  ps->parr[0].pnum = pnum;
  inited=true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Stage::showGrid(a::Node *node) {
  auto view= CC_GNF(PlayView, node, "view");
  auto nil = CC_CSV(c::Integer, "CV_Z");
  auto arr = mapGridPos(1.0f);
  auto pos=0;

  F__LOOP(it, arr) {
    auto sp= cx::reifySprite("z.png");
    view->cells[pos++]=sp;
    sp->setPosition(cx::vboxMID(*it));
    sp->setUserObject(CC_INT(nil));
    view->layer->addAtlasItem("game-pics", sp);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Stage::initOnline() {

  MGMS()->wsock()->listen( [=](ws::OdinEvent *evt) {
    this->onSocket(evt);
  });

  auto evt = new ws::OdinEvent(
      ws::MType::SESSION ,
      ws::EType::STARTED);
  c::RefPtr<ws::OdinEvent>
  rp(evt);

  ws::netSend(MGMS()->wsock(), evt);
  CCLOG("reply to server: session started ok");
}

//////////////////////////////////////////////////////////////////////////
//
void Stage::doIt(a::Node *node ) {
  auto active = MGMS()->isLive();
  int pnum;

  if (! active) {
    pnum= CC_GDV(c::Integer, state, "lastWinner");
  } else {
    pnum = CC_GDV(c::Integer, state, "pnum");
  }

  sendEx("/hud/update", j::json({
    { "running", active },
    { "pnum", pnum }
  }));

}

//////////////////////////////////////////////////////////////////////////
//
void Stage::onNet(ws::OdinEvent *evt) {

  switch (evt->code) {
    case ws::EType::RESTART:
      CCLOG("restarting a new game...");
      send("/net/restart");
    break;
    case ws::EType::STOP:
      if (MGMS()->isLive() ) {
        CCLOG("game will stop");
        send("/hud/timer/hide");
        onSess( evt);
        send("/net/stop");
      }
    break;
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Stage::onSess(ws::OdinEvent *evt) {
  auto ps= CC_GNF(Players, board->head, "players");
  auto grid= CC_GNF(Grid, board->head, "grid");
  auto src= evt->doco["source"];
  auto pnum= src["pnum"].get<j::json::number_integer_t>();
  auto cmd= src["cmd"];
  auto snd="";

  if (cmd.is_object()) {
    auto cell= cmd["cell"].get<j::json::number_integer_t>();
    auto cv= cmd["value"].get<j::json::number_integer_t>();
    if (cell >= 0 &&
        cell < grid->values.size()) {
      if (ps->parr[1].value == cv) {
        snd= "x_pick";
      } else {
        snd= "o_pick";
      }
      grid->values[cell] = cv;
      cx::sfxPlay(snd);
    }
  }

  if (pnum == 1 || pnum == 2) {} else { return; }

  switch (evt->code) {
    case ws::EType::POKE_MOVE:
      CCLOG("player %d: my turn to move", pnum);
      send("/hud/timer/show");
      state->setObject(CC_INT(pnum), "pnum");
    break;
    case ws::EType::POKE_WAIT:
      CCLOG("player %d: my turn to wait", pnum);
      send("/hud/timer/hide");
      // toggle color
      auto p2= pnum == 1 ? 2 : 1;
      state->setObject(CC_INT(p2), "pnum");
    break;
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Stage::onSocket(ws::OdinEvent *evt) {
  switch (evt->type) {
    case ws::MType::NETWORK:
      onNet(evt);
    break;
    case ws::MType::SESSION:
      onSess(evt);
    break;
  }
}




NS_END(tttoe)


