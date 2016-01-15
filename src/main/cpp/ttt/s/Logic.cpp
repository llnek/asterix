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
  humanNode = engine->getNodeList(HumanNode().typeId());
  robotNode = engine->getNodeList(RobotNode().typeId());
  arenaNode = engine->getNodeList(ArenaNode().typeId());
  boardNode = engine->getNodeList(BoardNode().typeId());
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

  auto ps = CC_GNLF(Players, boardNode, "players");
  auto sel = CC_GNLF(Select, boardNode, "click");
  auto grid= CC_GNLF(Grid, boardNode, "grid");
  auto ss= CC_GNLF(GVars,arenaNode,"slots");
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
      auto robot= CC_GNLF(Robot,robotNode,"robot");
      robot->board->syncState(grid->values, cp->value);
      int rc= robot->board->getFirstMove();
      if (rc < 0) {
        rc = ag::evalNegaMax<BD_SZ>(robot->board);
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

    if (ps->parr[other]->category == human) {
      SENDMSG("/hud/timer/show");
    }

    cx::sfxPlay(snd);
  }
}


NS_END(tttoe)


