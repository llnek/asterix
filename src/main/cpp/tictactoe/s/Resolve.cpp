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
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Resolve.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////
//
void Resolve::preamble() {
  _arena= _engine->getNodes("n/CSquares")[0];
  _board= _engine->getNodes("n/Grid")[0];
}

//////////////////////////////////////////////////////////////////////////
//
bool Resolve::update(float dt) {
  if (MGMS()->isLive()) {
    sync();
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::sync() {
  auto css= CC_GEC(CSquares, _arena, "n/CSquares");
  auto grid= CC_GEC(Grid, _board, "n/Grid");

  for (auto i=0; i < grid->vals.size(); ++i) {
    auto v= grid->vals[i];
    if (v != 0) {
      auto sq= css->sqs[i];
      if (sq->value == 0) {
        sq->toggle(v);
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::process(float dt) {

  auto ps = CC_GEC(Players, _board, "n/Players");
  auto gd = CC_GEC(Grid, _board, "n/Grid");
  auto winner= -1;
  ArrDim combo;

  for (auto i=1; i < ps->parr.size(); ++i) {
    if (checkWin(ps->parr[i], gd, combo)) {
      winner=i;
      break;
    }
  }

  if (winner > 0) {
    doWin(ps->parr[winner], combo);
  }
  else
  if (checkDraw(gd)) {
    doDraw();
  }
  else {
    auto &q= MGMS()->msgQueue();
    if (q.size() > 0) {
      auto s= q.front();
      q.pop();
      if ("forfeit" == s) {
        doForfeit();
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::doWin(Player *winner, const ArrDim &combo) {

  auto msg= j::json({
    {"pnum", winner->pnum },
    {"score", 1}
  });

  SENDMSGEX("/hud/score/update", &msg);
  showWinningIcons(combo);
  doDone(winner);
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::doDraw() {
  auto ps= CC_GEC(Players, _board, "n/Players");
  Player dummy;
  doDone(&dummy);
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::doForfeit() {
  auto css= CC_GEC(CSquares, _arena, "n/CSquares");
  auto ps= CC_GEC(Players, _board, "n/Players");
  auto ss= CC_GEC(GVars, _arena,"n/GVars");
  auto cur=ss->pnum;
  auto other= cur == 1 ? 2 : cur == 2 ? 1 : 0;

  auto loser = ps->parr[cur];
  auto win= ps->parr[other];

  auto msg = j::json({
    {"pnum", win->pnum },
    {"score", 1}
  });

  SENDMSGEX("/hud/score/update", &msg);

  //gray out the losing icons
  F__LOOP(it, css->sqs) {
    auto z = *it;
    if (z->value == loser->value) {
      z->flip();
    }
  }

  doDone(win);
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::showWinningIcons(const ArrDim &combo) {
  auto css= CC_GEC(CSquares, _arena, "n/CSquares");

  //flip the losing cells to gray
  for (int i=0; i < css->sqs.size(); ++i) {
    if (! (s::find(s::begin(combo), s::end(combo), i)
          != combo.end())) {
      auto z= css->sqs[i];
      if (z->value != 0) {
        z->flip();
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::doDone(Player *pobj) {

  auto ss= CC_GEC(GVars, _arena,"n/GVars");
  auto msg= j::json({
    {"winner", pobj->pnum  }
  });

  ss->lastWinner= pobj->pnum;

  SENDMSG("/hud/timer/hide");
  SENDMSGEX("/hud/end", &msg);

  cx::sfxPlay("game_end");
}

//////////////////////////////////////////////////////////////////////////////
//
bool Resolve::checkDraw(Grid *gd) {

  return ! (s::find(s::begin(gd->vals), s::end(gd->vals), 0)
    != gd->vals.end());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Resolve::checkWin(Player *p, Grid *game, ArrDim &combo) {

  //CCLOG("checking win for %s", p->color.c_str());

  F__LOOP(it, game->GOALS) {

    auto &g = *it;
    int cnt=0;

    for (auto i=0; i < g.size(); ++i) {
      auto pos = g[i];
      if (game->vals[pos] == p->value) {
        ++cnt;
      }
    }
    if (cnt == g.size()) {
       // found a winning combo, this guy wins!
      S__COPY(g, combo);
      return true;
    }
  }
  return false;
}



NS_END


