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

#include "core/Primitives.h"
#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "core/Odin.h"
#include "ash/Node.h"
#include "n/CObjs.h"
#include "utils.h"
#include "Stager.h"

NS_ALIAS(ws, fusii::odin)
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tttoe)


//////////////////////////////////////////////////////////////////////////////
Stager::Stager(not_null<EFactory*> f,
    not_null<c::Dictionary*> d)

  : f::BaseSystem<EFactory>(f, d) {

  this->inited=false;
  SNPTR(board)
}

//////////////////////////////////////////////////////////////////////////////
//
void Stager::addToEngine(not_null<a::Engine*> e) {

  CCLOG("adding system: Stager");

  BoardNode n;
  board = e->getNodeList(n.typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Stager::onUpdate(float dt) {

  if (cx::isTransitioning()) {
    return false;
  }

  auto& n = board->head;

  if (NNP(n)) {

    if (! inited) {
      onceOnly(n);
    }

    if (MGMSOK()) {
      doIt(n, dt);
    }

  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Stager::onceOnly(a::Node* node) {
  auto ps = CC_GNF(Players, node, "players");
  auto human = CC_CSV(c::Integer, "HUMAN");
  auto bot= CC_CSV(c::Integer,"BOT");
  auto pnum = 0;

  showGrid(node);

  if (MGMS()->isOnline()) {
    initOnline();
  } else {
    pnum= CCRANDOM_0_1() > 0.5f ? 1 : 2;
    // randomly choose
    if (ps->parr[pnum].category == human) {
      MGMS()->sendMsg("/hud/timer/show");
    }
    else
    if (ps->parr[pnum].category == bot) {
      //noop
    }
    ps->parr[0]= ps->parr[pnum];
  }

  inited=true;
  state->setObject(CC_INT(pnum), "pnum");
}

//////////////////////////////////////////////////////////////////////////////
//
void Stager::showGrid(a::Node* node) {
  auto view= CC_GNF(PlayView, node, "view");
  auto nil = CC_CSV(c::Integer, "CV_Z");
  auto arr = mapGridPos(1.0f);
  auto pos=0;

  F__LOOP(it, arr) {
    auto sp= cx::reifySprite("z.png");
    sp->setPosition(cx::vboxMID(*it));
    sp->setUserObject(CC_INT(nil));
    view->layer->addAtlasItem("game-pics", sp);
    view->cells[pos++]=sp;
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Stager::initOnline() {

  MGMS()->wsock()->listen( [=](ws::OdinEvent* evt) {
    this->onSocket(evt);
  });

  auto evt = new ws::OdinEvent( ws::MType::SESSION, ws::EType::STARTED);
    c::RefPtr<ws::OdinEvent> rp(evt);

  ws::netSend(MGMS()->wsock(), evt);
  CCLOG("reply to server: session started ok");
}

//////////////////////////////////////////////////////////////////////////
//
void Stager::doIt(a::Node* node, float dt) {

  auto pnum = CC_GDV(c::Integer, state, "pnum");
  auto active = MGMSOK();

  if (! active) {
    pnum= CC_GDV(c::Integer, state, "lastWinner");
  }

  j::json msg = j::json( {
    { "running", active },
    { "pnum", pnum }
  });

  MGMS()->sendMsgEx("/hud/update", &msg);
}

//////////////////////////////////////////////////////////////////////////
//
void Stager::onNet(ws::OdinEvent* evt) {

  switch (evt->code) {
    case ws::EType::RESTART:
      CCLOG("restarting a new game...");
      MGMS()->sendMsg("/net/restart");
    break;
    case ws::EType::STOP:
      if (MGMSOK()) {
        CCLOG("game will stop");
        MGMS()->sendMsg("/hud/timer/hide");
        onSess( evt);
        MGMS()->sendMsgEx("/net/stop", evt);
      }
    break;
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Stager::onSess(ws::OdinEvent* evt) {
  auto ps= CC_GNF(Players, board->head, "players");
  auto grid= CC_GNF(Grid, board->head, "grid");
  auto src = evt->doco["source"];
  auto pnum = src["pnum"].get<j::json::number_integer_t>();
  auto cmd = src["cmd"];
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
      MGMS()->sendMsg("/hud/timer/show");
      state->setObject(CC_INT(pnum), "pnum");
    break;
    case ws::EType::POKE_WAIT:
      CCLOG("player %d: my turn to wait", pnum);
      MGMS()->sendMsg("/hud/timer/hide");
      // toggle color
      auto p2= pnum == 1 ? 2 : 1;
      state->setObject(CC_INT(p2), "pnum");
    break;
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Stager::onSocket(ws::OdinEvent* evt) {
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


