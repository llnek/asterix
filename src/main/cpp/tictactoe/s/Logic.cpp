// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Ken Leung. All rights reserved.

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
  auto ents= engine->getEntities("n/SmartAI");
  robot= ents.size() > 0 ? ents[0] : CC_NIL;
  arena= engine->getEntities("n/CSquares")[0];
  board= engine->getEntities("n/Grid")[0];
}

//////////////////////////////////////////////////////////////////////////
//
bool Logic::update(float dt) {
  if (MGMS()->isLive() &&
      !MGMS()->isOnline()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Logic::process(float dt) {

  auto sel = CC_GEC(CellPos, board, "n/CellPos");
  auto ps = CC_GEC(Players, board, "n/Players");
  auto grid= CC_GEC(Grid, board, "n/Grid");
  auto ss= CC_GEC(GVars,arena,"n/GVars");
  auto bot= CC_CSV(c::Integer, "BOT");
  auto cfg= MGMS()->getLCfg()->getValue();
  // current player
  auto cp= ps->parr[ss->pnum];

  if (cp->category == bot) {
    if (ENP(botTimer)) {
      botTimer = cx::reifyTimer(
          MGML(),
          JS_FLOAT(cfg["ROBOT+DELAY"]));
    }
    else
    if (cx::timerDone(botTimer)) {
      auto bd= CC_GEC(TTToe,robot,"n/SmartAI");
      bd->syncState(grid->vals, cp->value);
      int rc= bd->getFirstMove();
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

  auto ps= CC_GEC(Players, board, "n/Players");
  auto ss= CC_GEC(GVars,arena,"n/GVars");
  auto human= CC_CSV(c::Integer,"HUMAN");
  auto other=0;
  auto snd="";

  if ((pos >= 0 && pos < grid->vals.size()) &&
      0 == grid->vals[pos]) {

    SENDMSG("/hud/timer/hide");

    if (ss->pnum == 1) {
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


NS_END


