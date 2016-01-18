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
  robot= engine->getNodeList(RobotNode().typeId());
  arena= engine->getNodeList(ArenaNode().typeId());
  board= engine->getNodeList(BoardNode().typeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Logic::update(float dt) {
  if (MGMS()->isLive() &&
      !MGMS()->isOnline()) {
    doIt(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Logic::doIt(float dt) {

  auto ps = CC_GNLF(Players, board, "players");
  auto sel = CC_GNLF(CellPos, board, "select");
  auto grid= CC_GNLF(Grid, board, "grid");
  auto ss= CC_GNLF(GVars,arena,"slots");
  auto bot= CC_CSV(c::Integer, "BOT");
  auto cfg= MGMS()->getLCfg()->getValue();
  auto pnum= ss->pnum;
  auto cp= ps->parr[pnum];

  if (cp->category == bot) {
    if (ENP(botTimer)) {
      // create some small delay...
      botTimer = cx::reifyTimer(
          MGML(),
          JS_FLOAT(cfg["ROBOT+DELAY"]));
    }
    else
    if (cx::timerDone(botTimer)) {
      auto bd= CC_GNLF(TTToe,robot,"robot");
      int rc;
      bd->syncState(grid->vals, cp->value);
      rc= bd->getFirstMove();
      if (rc < 0) {
        rc = ag::evalNegaMax<BD_SZ>(bd);
      }
      sync(rc, cp->value, grid);
      cx::undoTimer(botTimer);
      SNPTR(botTimer)
    }
  }
  else
  if (sel->cell >= 0) {
    //possibly a valid click ? handle it
    sync(sel->cell, cp->value, grid);
  }

  sel->cell= -1;
}

//////////////////////////////////////////////////////////////////////////
//
void Logic::sync(int pos, int value, Grid *grid) {

  auto ps= CC_GNLF(Players, board, "players");
  auto ss= CC_GNLF(GVars,arena,"slots");
  auto nil = 0;
  auto human= CC_CSV(c::Integer,"HUMAN");
  auto cur = ss->pnum;
  auto other=0;
  auto snd="";

  if ((pos >= 0 && pos < grid->vals.size()) &&
      nil == grid->vals[pos]) {

    SENDMSG("/hud/timer/hide");

    if (cur == 1) {
      snd= "x_pick";
      other = 2;
    } else {
      snd= "o_pick";
      other = 1;
    }

    // switch player
    grid->vals[pos] = value;
    ss->pnum= other;
    auto msg= j::json({
        {"category", ps->parr[other]->category},
        {"running", MGMS()->isLive() },
        {"pnum", other}
        });
    SENDMSGEX("/hud/update",&msg);

    if (ps->parr[other]->category == human) {
      SENDMSG("/hud/timer/show");
    }

    cx::sfxPlay(snd);
  }
}


NS_END(tttoe)


