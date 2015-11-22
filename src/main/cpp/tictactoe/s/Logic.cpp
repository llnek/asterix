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

#include "dbox/json11.hpp"
#include "algos/NegaMax.h"
#include "x2d/GameScene.h"
#include "core/Odin.h"
#include "n/CObjs.h"
#include "Logic.h"
NS_ALIAS(ws, fusii::odin)
NS_BEGIN(tttoe)


//////////////////////////////////////////////////////////////////////////
//
Logic::Logic(not_null<EFactory*> f, not_null<c::Dictionary*> d)

  : f::BaseSystem(f,d) {

  SNPTR(botTimer)
  SNPTR(board)
}

//////////////////////////////////////////////////////////////////////////
//
void Logic::addToEngine(not_null<a::Engine*> e) {
  BoardNode n;
  board = e->getNodeList(n.typeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Logic::onUpdate(float dt) {
  auto node= board->head;
  if (MGMS()->isLive() && NNP(node)) {
    doIt(node, dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Logic::doIt(a::Node* node, float dt) {

  auto pnum= CC_GDV(c::Integer, state, "pnum");
  auto ps = CC_GNF(Players, node, "players");
  auto bot= CC_CSV(c::Integer, "BOT");
  auto cp= ps->parr[pnum];

  auto sel= CC_GNF(UISelection, node, "selection");
  auto grid= CC_GNF(Grid, node, "grid");
  auto robot= CC_GNF(SmartAlgo, node, "robot");

  //handle online play
  if (MGMS()->isLive()) {
    //if the mouse click is from the valid user, handle it
    if (pnum == cp.pnum) {
      enqueue(node, sel->cell, cp.value, grid);
    }
  }
  else
  if (cp.category == bot) {
    // for the bot, create some small delay...
    if (NNP(botTimer) &&
        cx::timerDone(botTimer)) {
      auto bd= robot->board;
      int rc=0;
      bd->syncState(grid->values, cp.value);
      rc= bd->getFirstMove();
      if (rc < 0) { rc = fusii::algos::evalNegaMax<BD_SZ>(bd); }
      enqueue(node, rc, cp.value, grid);
      cx::undoTimer(botTimer);
      SNPTR(botTimer)
    } else {
      botTimer = cx::reifyTimer(MGML(), 0.6);
    }
  }
  else
  if (sel->cell >= 0) {
    //possibly a valid click ? handle it
    enqueue(node, sel->cell, cp.value, grid);
  }

  sel->cell= -1;
}

//////////////////////////////////////////////////////////////////////////
//
void Logic::enqueue(a::Node* node, int pos, int value, Grid* grid) {

  auto cur = CC_GDV(c::Integer, state, "pnum");
  auto nil = CC_CSV(c::Integer, "CV_Z");
  auto ps= CC_GNF(Players, node, "players");
  auto human= CC_CSV(c::Integer,"HUMAN");
  auto other=0;
  auto snd="";

  if ((pos >= 0 && pos < grid->values.size()) &&
      nil == grid->values[pos]) {

    MGMS()->sendMsg("/hud/timer/hide");

    if (MGMS()->isLive()) {
      onEnqueue(node, cur, pos, grid);
    } else {
      if (cur == 1) {
        snd= "x_pick";
        other = 2;
      } else {
        snd= "o_pick";
        other = 1;
      }

      // switch player
      state->setObject(CC_INT(other), "pnum");
      grid->values[pos] = value;
      cx::sfxPlay(snd);

      if (ps->parr[other].category == human) {
        MGMS()->sendMsg("/hud/timer/show");
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Logic::onEnqueue(a::Node* node, int pnum, int cell, Grid* grid) {
  auto ps = CC_GNF(Players, node, "players");
  auto body = j::Json::object {
    { "color", ps->parr[pnum].color },
    { "value", ps->parr[pnum].value },
    { "grid", grid->values },
    { "cell", cell }
  };

  auto snd = pnum == 1 ? "x_pick" : "o_pick";
  auto c = ws::EType::PLAY_MOVE;
  auto t = ws::MType::SESSION;

  ws::netSend(MGMS()->wsock(), new ws::OdinEvent(t,c, body));

  state->setObject(CC_INT(0), "pnum");
  cx::sfxPlay(snd);
}





NS_END(tttoe)

