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

#include "base/ccRandom.h"
#include "core/CCSX.h"
#include "TTToe.h"
#include <math.h>

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tttoe)

BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
bool not_any(const ArrCells &arr, int v) {
  for (int i=0; i < arr.size(); ++i) {
    if (arr[i] == v) { return false; }
  }
  return arr.size() > 0;
}

//////////////////////////////////////////////////////////////////////////////
//
bool every(const ArrCells &arr, int v) {
  for (int i=0; i < arr.size(); ++i) {
    if (arr[i] != v) { return false; }
  }
  return arr.size() > 0;
}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
TTToe::TTToe(int nil, int p1v, int p2v) {
  this->actors = {nil, p1v, p2v};
  this->GOALS= mapGoalSpace();
  this->CV_Z= nil;
}

//////////////////////////////////////////////////////////////////////////////
//
bool TTToe::isNil(int cellv) {
  return cellv == this->CV_Z;
}

//////////////////////////////////////////////////////////////////////////////
// if brand new game, just make a random move
int TTToe::getFirstMove() {
  auto sz= grid.size();
  return sz > 0 && every(grid, CV_Z) ? cx::randInt(sz) : -1;
}

//////////////////////////////////////////////////////////////////////////////
//
void TTToe::syncState(const ArrCells &seed, int actor) {
  s::copy(s::begin(seed), s::end(seed), s::begin(grid));
  actors[0] = actor;
}

//////////////////////////////////////////////////////////////////////////////
// find set of empty slots
const s_vec<int>
TTToe::getNextMoves(not_null<ag::FFrame<BD_SZ>*> snap) {

  auto sz= snap->state.size();
  s_vec<int> rc;

  for (int pos= 0; pos < sz; ++pos) {
    if (isNil(snap->state[pos])) {
      rc.push_back(pos);
    }
  }

  return rc;
}

//////////////////////////////////////////////////////////////////////////////
//
void TTToe::undoMove(not_null<ag::FFrame<BD_SZ>*> snap, int move) {
  assert(move >= 0 && move < snap->state.size());
  snap->state[move] = CV_Z;
}

//////////////////////////////////////////////////////////////////////////////
//
void TTToe::makeMove(not_null<ag::FFrame<BD_SZ>*> snap, int move) {
  assert(move >= 0 && move < snap->state.size());
  if (isNil(snap->state[move])) {
    snap->state[move] = snap->cur;
  } else {
    throw "cell [" + s::to_string(move) + "] is not free";
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void TTToe::switchPlayer(not_null<ag::FFrame<BD_SZ>*> snap) {
  auto t = snap->cur;
  snap->cur= snap->other;
  snap->other=t;
}

//////////////////////////////////////////////////////////////////////////////
//
int TTToe::getOtherPlayer(int pv) {
  if (pv == actors[1]) {
    return actors[2];
  }

  if (pv == actors[2]) {
    return actors[1];
  }

  return CV_Z;
}

//////////////////////////////////////////////////////////////////////////////
//
owner<ag::FFrame<BD_SZ>*>  TTToe::takeFFrame() {
  auto ff = mc_new( ag::FFrame<BD_SZ> );

  s::copy(s::begin(grid), s::end(grid), s::begin(ff->state));
  ff->other= getOtherPlayer(actors[0]);
  ff->cur= actors[0];
  ff->lastBestMove= -1;

  return ff;
}

//////////////////////////////////////////////////////////////////////////////
//
int TTToe::evalScore(not_null<ag::FFrame<BD_SZ>*> snap) {
  // if we lose, return a nega value
  F__LOOP(it, GOALS) {
    if (testWin(snap->state, snap->other, *it)) {
      return -100;
    }
  }
  return 0;
}

//////////////////////////////////////////////////////////////////////////////
//
bool TTToe::isOver(not_null<ag::FFrame<BD_SZ>*> snap) {

  F__LOOP(it, GOALS) {
    auto &t = *it;
    if (testWin(snap->state, snap->other, t) ||
        testWin(snap->state, snap->cur, t)) {
      return true;
    }
  }
  return isStalemate(snap);
}

//////////////////////////////////////////////////////////////////////////////
//
bool TTToe::isStalemate(not_null<ag::FFrame<BD_SZ>*> snap) {
  return not_any(snap->state, CV_Z);
}

//////////////////////////////////////////////////////////////////////////////
//
int TTToe::getWinner(not_null<ag::FFrame<BD_SZ>*> snap, ArrDim &combo) {

  int win= -1;
  F__LOOP(it, GOALS) {
    auto &t= *it;
    if (testWin(snap->state, snap->other, t)) {
      win=snap->other;
    }
    else
    if (testWin(snap->state, snap->cur, t)) {
      win=snap->cur;
    }
    if (win > 0) {
      s::copy(s::begin(t),s::end(t),s::begin(combo));
      break;
    }
  }
  return win;
}

//////////////////////////////////////////////////////////////////////////////
//
bool TTToe::testWin(const ArrCells &vs, int actor, const ArrDim &g) {

  int cnt=g.size();
  for (int n= 0; n < g.size(); ++n) {
    if (actor == vs[g[n]]) {
      --cnt;
    }
  }
  return cnt == 0;
}


NS_END(tttoe)

