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

//#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "core/Odin.h"
#include "n/CObjs.h"
#include "utils.h"
#include "Stage.h"
#include "p/Game.h"

NS_ALIAS(ws, fusii::odin)
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
void Stage::preamble() {
  BoardNode b;
  ArenaNode a;

  boardNode = engine->getNodeList(b.typeId());
  arenaNode = engine->getNodeList(a.typeId());
  onceOnly();
}

//////////////////////////////////////////////////////////////////////////////
//
bool Stage::update(float dt) {

  if (MGMS()->isLive()) {
    doIt();
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Stage::onceOnly() {

  auto ps=CC_GNLF(Players, boardNode, "players");
  auto ss=CC_GNLF(GVars,arenaNode,"slots");
  auto human = CC_CSV(c::Integer, "HUMAN");
  auto bot= CC_CSV(c::Integer,"BOT");
  auto pnum = 0;

  showGrid();

  if (MGMS()->isOnline()) {
    initOnline();
  } else {
    pnum= c::rand_0_1() > 0.5f ? 1 : 2;
    // randomly choose
    if (ps->parr[pnum].category == human) {
      SENDMSG("/hud/timer/show");
    }
    else
    if (ps->parr[pnum].category == bot) {
      //noop
    }
  }

  ps->parr[0].pnum = pnum;
  ss->pnum= pnum;
}

//////////////////////////////////////////////////////////////////////////////
//
void Stage::showGrid() {
  auto view= CC_GNLF(PlayView, boardNode, "view");
  auto nil = CC_CSV(c::Integer, "CV_Z");
  auto arr = mapGridPos(1);
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

  MGMS()->wsock()->listen([=](ws::OdinEvent *evt) {
    this->onSocket(evt);
  });

  auto ctx= (GCXX*) MGMS()->getCtx();
  if (ctx->count > 1) {

    auto evt = new ws::OdinEvent(
        ws::MType::SESSION ,
        ws::EType::REPLAY);
    c::RefPtr<ws::OdinEvent>
    rp(evt);
    ws::netSend(MGMS()->wsock(), evt);
    CCLOG("acknowledge to server: replay please");

  } else {

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
void Stage::doIt() {

  auto ss= CC_GNLF(GVars,arenaNode,"slots");
  auto active = MGMS()->isLive();
  int pnum;

  if (! active) {
    pnum= ss->lastWinner;
  } else {
    pnum = ss->pnum;
  }

  auto msg= j::json({
    { "running", active },
    { "pnum", pnum }
      });
  SENDMSGEX("/hud/update", &msg);
}

//////////////////////////////////////////////////////////////////////////
//
void Stage::onNet(ws::OdinEvent *evt) {

  switch (evt->code) {
    case ws::EType::RESTART:
      CCLOG("restarting a new game...");
      SENDMSG("/net/restart");
    break;
    case ws::EType::STOP:
      if (MGMS()->isLive() ) {
        CCLOG("game will stop");
        SENDMSG("/hud/timer/hide");
        onSess( evt);
        SENDMSG("/net/stop");
      }
    break;
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Stage::onSess(ws::OdinEvent *evt) {
  auto ps= CC_GNLF(Players, boardNode, "players");
  auto grid= CC_GNLF(Grid, boardNode, "grid");
  auto ss= CC_GNLF(GVars,arenaNode,"slots");
  auto src= evt->doco["source"];
  auto pnum= JS_INT(src["pnum"]);
  auto cmd= src["cmd"];
  auto snd="";

  if (cmd.is_object()) {
    auto cell= JS_INT(cmd["cell"]);
    auto cv= JS_INT(cmd["value"]);
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


