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
#include "algos/NegaMax.h"
#include "core/XConfig.h"
#include "core/Odin.h"
#include "core/JSON.h"
#include "Logic.h"

NS_ALIAS(ag, fusii::algos)
NS_ALIAS(ws, fusii::odin)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////
//
void Logic::preamble() {
  BoardNode b;
  ArenaNode a;

  arenaNode = engine->getNodeList(a.typeId());
  boardNode = engine->getNodeList(b.typeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Logic::update(float dt) {
  if (MGMS()->isLive() ) {
    doIt(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Logic::doIt(float dt) {

  auto sel= CC_GNLF(CellPos, boardNode, "cellpos");
  auto robot= CC_GNLF(Robot, boardNode, "robot");
  auto grid= CC_GNLF(Grid, boardNode, "grid");
  auto ps = CC_GNLF(Players, boardNode, "players");
  auto ss= CC_GNLF(GVars,arenaNode,"slots");
  auto bot= CC_CSV(c::Integer, "BOT");
  auto cfg= MGMS()->getLCfg()->getValue();
  auto pnum= ss->pnum;
  auto &cp= ps->parr[pnum];

  if (MGMS()->isOnline()) {
    //if the mouse click is from the valid user, handle it
    if (pnum == cp.pnum) {
      enqueue(sel->cell, cp.value, grid);
    }
  }
  else
  if (cp.category == bot) {
    if (ENP(botTimer)) {
      // create some small delay...
      botTimer = cx::reifyTimer(
          MGML(),
          JS_FLOAT(cfg["ROBOT+DELAY"]));
    }
    else
    if (cx::timerDone(botTimer)) {
      auto bd= robot->board;
      int rc=0;
      bd->syncState(grid->values, cp.value);
      rc= bd->getFirstMove();
      if (rc < 0) {
        rc = ag::evalNegaMax<BD_SZ>(bd);
      }
      enqueue( rc, cp.value, grid);
      cx::undoTimer(botTimer);
      SNPTR(botTimer)
    }
  }
  else
  if (sel->cell >= 0) {
    //possibly a valid click ? handle it
    enqueue( sel->cell, cp.value, grid);
  }

  sel->cell= -1;
}

//////////////////////////////////////////////////////////////////////////
//
void Logic::enqueue( int pos, int value, Grid *grid) {

  auto ps= CC_GNLF(Players, boardNode, "players");
  auto ss= CC_GNLF(GVars,arenaNode,"slots");
  auto nil = CC_CSV(c::Integer, "CV_Z");
  auto human= CC_CSV(c::Integer,"HUMAN");
  auto cur = ss->pnum;
  auto other=0;
  auto snd="";

  if ((pos >= 0 && pos < grid->values.size()) &&
      nil == grid->values[pos]) {

    SENDMSG("/hud/timer/hide");

    if (MGMS()->isOnline()) {
      onEnqueue( cur, pos, grid);
    } else {

      if (cur == 1) {
        snd= "x_pick";
        other = 2;
      } else {
        snd= "o_pick";
        other = 1;
      }

      // switch player
      grid->values[pos] = value;
      ss->pnum= other;

      if (ps->parr[other].category == human) {
        SENDMSG("/hud/timer/show");
      }

      cx::sfxPlay(snd);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Logic::onEnqueue(int pnum, int cell, Grid *grid) {
  auto ps = CC_GNLF(Players, boardNode, "players");
  auto ss= CC_GNLF(GVars,arenaNode,"slots");
  auto body = j::json({
    { "color", ps->parr[pnum].color },
    { "value", ps->parr[pnum].value },
    { "grid", grid->values },
    { "cell", cell }
  });
  auto snd = pnum == 1 ? "x_pick" : "o_pick";
  auto c = ws::EType::PLAY_MOVE;
  auto t = ws::MType::SESSION;

  auto evt= mc_new_3(ws::OdinEvent, t,c, body);
  c::RefPtr<ws::OdinEvent>
  rp(evt);

  ss->pnum=0;
  cx::sfxPlay(snd);
    ws::netSend(MGMS()->wsock(), (ws::OdinEvent*)evt);
}


NS_END(tttoe)


