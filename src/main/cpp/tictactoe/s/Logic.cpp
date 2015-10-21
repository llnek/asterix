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

#include "Logic.h"
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////
//
Logic::Logic(not_null<EFactory*> f, not_null<c::Dictionary*> d)

  : f::BaseSystem(f,d) {

  SNPTR(botTimer)
}

//////////////////////////////////////////////////////////////////////////
//
void Logic::AddToEngine(not_null<a::Engine*> e) {
  BoardNode n;
  board = e->GetNodeList(n.TypeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Logic::OnUpdate(float dt) {
  auto node= board->head;
  if (MGMS()->IsRunning() && NNP(node)) {
    DoIt(node, dt);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Logic::DoIt(a::Node* node, float dt) {

  auto pnum= CC_GDV(c::Integer, state, "actor");
  auto ps = CC_GNF(Players, node, "players");
  auto bot= CC_CSV(c::Integer, "BOT");
  auto cp= ps->parr[cur];

  auto sel= CC_GNF(UISelection, node, "selection");
  auto board= CC_GNF(Board, node, "board");
  auto grid= CC_GNF(Grid, node, "grid");
  auto bot= CC_GNF(SmartAlgo, node, "robot");

  //handle online play
  if (MGMS()->IsOnline()) {
    //if the mouse click is from the valid user, handle it
    if (NNP(cp) && (pnum == cp->pnum)) {
      Enqueue(sel->cell, cp->value, grid);
    }
  }
  else
  if (cp->category == bot) {
    // for the bot, create some small delay...
    if (NNP(botTimer) &&
        cx::TimerDone(botTimer)) {
      auto bd= bot->algo->board;
      bd->SyncState(grid->values, ps->parr[cur]->value);
      rc= bd->GetFirstMove();
      if (rc < 0) { rc = bot->algo->Eval(); }
      Enqueue(rc,cp->value, grid);
      cx::UndoTimer(botTimer);
      SNPTR(botTimer)
    } else {
      botTimer = cx::CreateTimer(MGML(), 0.6);
    }
  }
  else
  if (sel->cell >= 0) {
    //possibly a valid click ? handle it
    Enqueue(sel->cell, cp->value, grid);
  }

  sel->cell= -1;
}

//////////////////////////////////////////////////////////////////////////
//
void Logic::Enqueue(int pos, int value, Grid* grid) {

  auto cur = CC_GDV(c::Integer, state, "actor");
  auto nil = CC_CSV(c::Integer, "CV_Z");
  auto human= CC_CSV(c::Integer,"HUMAN");
  auto pnum=0;
  auto snd="";

  if ((pos >= 0 && pos < grid->values.Size()) &&
      nil == grid->values[pos]) {

    MGML()->SendMsg("/hud/timer/hide");

    if (MGMS()->IsOnline()) {
      OnEnqueue(cur, pos, grid);
    } else {
      if (cur == 1) {
        snd= "x_pick";
        pnum = 2;
      } else {
        snd= "o_pick";
        pnum = 1;
      }

      state->setObject(c::Integer::create(pnum), "actor");
      grid->values[pos] = value;
      cx::SfxPlay(snd);

      if (ps->parr[pnum]->category == human) {
        MGMS()->SendMsg("/hud/timer/show");
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Logic::OnEnqueue(int pnum, int cell, Grid* grid) {
  auto ps = CC_GNF(Players, node, "players");
  /*TODO: fix me
  const src= {
    color: ps->parr[pnum]->color,
    value: ps->parr[pnum]->value,
    grid: grid->values,
    cell: cell
  }
  */
  auto snd = pnum == 1 ? "x_pick" : "o_pick";
  /*TODO: fix me
  evt= {
    source: sjs.jsonfy(src),
    type: evts.MSG_SESSION,
    code: evts.PLAY_MOVE
  };
  MGMS()->NetSend(evt);
  */
  state->setObject(c::Integer::create(0), "actor");
  cx::SfxPlay(snd);
}





NS_END(tttoe)

