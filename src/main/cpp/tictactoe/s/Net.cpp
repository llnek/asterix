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
#include "core/Odin.h"
#include "n/C.h"
#include "n/lib.h"
#include "Net.h"

NS_ALIAS(ws, fusii::odin)
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
bool Net::update(float dt) {
  if (MGMS()->isLive()) {
    process();
    sync();
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Net::preamble() {
  board = engine->getNodeList(BoardNode().typeId());
  arena = engine->getNodeList(ArenaNode().typeId());
  auto ps= CC_GNLF(Players,board,"players");
  auto ss= CC_GNLF(GVars, arena, "slots");
  ss->pnum= 0; // no one is current yet
  initOnline();
}

//////////////////////////////////////////////////////////////////////////
//
void Net::initOnline() {

  MGMS()->wsock()->listen([=](ws::OdinEvent *evt) {
    this->onSocket(evt);
  });

  auto ctx= MGMS()->getCtx();
  if (ctx->count > 1) {
    // a replay
    auto evt = new ws::OdinEvent(
        ws::MType::SESSION ,
        ws::EType::REPLAY);
    c::RefPtr<ws::OdinEvent>
    rp(evt);
    ws::netSend(MGMS()->wsock(), evt);
    CCLOG("acknowledge to server: replay please");
  } else {
    // new game
    auto evt = new ws::OdinEvent(
        ws::MType::SESSION ,
        ws::EType::STARTED);
    c::RefPtr<ws::OdinEvent>
    rp(evt);
    ws::netSend(MGMS()->wsock(), evt);
    CCLOG("reply to server: session started ok");
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Net::sync() {

  auto ss= CC_GNLF(GVars,arena,"slots");
  auto active = MGMS()->isLive();
  int pnum;

  if (! active) {
    pnum= ss->lastWinner;
  } else {
    pnum = ss->pnum;
  }

  auto msg= j::json({
    {"category", CC_CSV(c::Integer, "NETP")},
    { "running", active },
    { "pnum", pnum }
  });

  SENDMSGEX("/hud/update", &msg);
}

//////////////////////////////////////////////////////////////////////////
//
void Net::process() {
  auto ps = CC_GNLF(Players, board, "players");
  auto sel = CC_GNLF(CellPos, board, "select");
  auto grid= CC_GNLF(Grid, board, "grid");
  auto ss= CC_GNLF(GVars,arena, "slots");
  auto pos = sel->cell;
  auto cur = ss->pnum;

  // is it really your turn?
  if (cur < 1 ||
      cur != ps->parr[0]->pnum)
  { return;}

  if ((pos >= 0 && pos < grid->vals.size()) &&
      0 == grid->vals[pos])
  {}
  else
  { return; }

  //ok, you clicked a square
  SENDMSG("/hud/timer/hide");

  auto snd = cur == 1 ? "x_pick" : "o_pick";
  auto evt= new ws::OdinEvent(
    ws::MType::SESSION ,
    ws::EType::PLAY_MOVE ,
    j::json({
      { "color", ps->parr[cur]->color },
      { "value", ps->parr[cur]->value },
      { "grid", grid->vals },
      { "cell", pos }
    }));
  c::RefPtr<ws::OdinEvent>
  rp(evt);

  //reset current player, wait for server to reset
  cx::sfxPlay(snd);
  ss->pnum=0;

  ws::netSend(MGMS()->wsock(), evt);
}

//////////////////////////////////////////////////////////////////////////
//
void Net::onNet(ws::OdinEvent *evt) {

  switch (evt->code) {
    case ws::EType::RESTART:
      CCLOG("restarting a new game...");
      SENDMSG("/net/restart");
    break;
    case ws::EType::STOP:
      if (MGMS()->isLive() ) {
        CCLOG("game will stop");
        SENDMSG("/hud/timer/hide");
        onSess(evt);
        SENDMSG("/net/stop");
      }
    break;
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Net::onSess(ws::OdinEvent *evt) {
  auto ps= CC_GNLF(Players, board, "players");
  auto grid= CC_GNLF(Grid, board, "grid");
  auto ss= CC_GNLF(GVars,arena,"slots");
  auto src= evt->doco["source"];
  auto pnum= JS_INT(src["pnum"]);
  auto cmd= src["cmd"];
  auto snd="";

  if (cmd.is_object()) {
    auto cell= JS_INT(cmd["cell"]);
    auto cv= JS_INT(cmd["value"]);
    if (cell >= 0 &&
        cell < grid->vals.size()) {
      if (ps->parr[1]->value == cv) {
        snd= "x_pick";
      } else {
        snd= "o_pick";
      }
      grid->vals[cell] = cv;
      cx::sfxPlay(snd);
    }
  }

  if (pnum < 1) { return; }

  switch (evt->code) {
    case ws::EType::POKE_MOVE:
      CCLOG("player %d: my turn to move", pnum);
      SENDMSG("/hud/timer/show");
      ss->pnum= pnum;
    break;
    case ws::EType::POKE_WAIT:
      CCLOG("player %d: my turn to wait", pnum);
      SENDMSG("/hud/timer/hide");
      // toggle color
      ss->pnum = pnum == 1 ? 2 : 1;
    break;
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Net::onSocket(ws::OdinEvent *evt) {
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


