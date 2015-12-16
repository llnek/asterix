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
#include "n/GNodes.h"
#include "Logic.h"

NS_ALIAS(ag, fusii::algos)
NS_ALIAS(ws, fusii::odin)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////
//
Logic::Logic(not_null<EFactory*> f, not_null<c::Dictionary*> d)
  : BaseSystem<EFactory>(f,d) {
}

//////////////////////////////////////////////////////////////////////////
//
void Logic::addToEngine(not_null<a::Engine*> e) {
  BoardNode n;
  board = e->getNodeList(n.typeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Logic::update(float dt) {
  CCLOG("Logic::update()");
  auto n= board->head;
  if (MGMS()->isLive() ) {
    doIt(n, dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Logic::doIt(a::Node *node, float dt) {

  auto sel= CC_GNF(UISelection, node, "selection");
  auto robot= CC_GNF(SmartAlgo, node, "robot");
  auto grid= CC_GNF(Grid, node, "grid");

  auto pnum= CC_GDV(c::Integer, state, "pnum");
  auto ps = CC_GNF(Players, node, "players");
  auto bot= CC_CSV(c::Integer, "BOT");
  auto &cp= ps->parr[pnum];

  //handle online play
  if (MGMS()->isOnline()) {
    //if the mouse click is from the valid user, handle it
    if (pnum == cp.pnum) {
      enqueue(node, sel->cell, cp.value, grid);
    }
  }
  else
  if (cp.category == bot) {
    if (ENP(botTimer)) {
      // for the bot, create some small delay...
      botTimer = cx::reifyTimer(MGML(), 0.6f);
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
      enqueue(node, rc, cp.value, grid);
      cx::undoTimer(botTimer);
      SNPTR(botTimer)
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
void Logic::enqueue(a::Node *node, int pos, int value, Grid *grid) {

  auto cur = CC_GDV(c::Integer, state, "pnum");
  auto nil = CC_CSV(c::Integer, "CV_Z");
  auto ps= CC_GNF(Players, node, "players");
  auto human= CC_CSV(c::Integer,"HUMAN");
  auto other=0;
  auto snd="";

  if ((pos >= 0 && pos < grid->values.size()) &&
      nil == grid->values[pos]) {

    send("/hud/timer/hide");

    if (MGMS()->isOnline()) {
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
        send("/hud/timer/show");
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Logic::onEnqueue(a::Node *node, int pnum, int cell, Grid *grid) {
  auto ps = CC_GNF(Players, node, "players");
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

  state->setObject(CC_INT(0), "pnum");
  cx::sfxPlay(snd);

  ws::netSend(MGMS()->wsock(), evt);
}


NS_END(tttoe)


