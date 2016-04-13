// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

#include "base/ccRandom.h"
#include "core/CCSX.h"
#include "TTToe.h"
#include <math.h>

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tttoe)

BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
bool not_any(const ArrCells &arr, int v) {
  for (auto i=0; i < arr.size(); ++i) {
    if (arr[i] == v) { return false; }
  }
  return arr.size() > 0;
}

//////////////////////////////////////////////////////////////////////////////
//
bool every(const ArrCells &arr, int v) {
  for (auto i=0; i < arr.size(); ++i) {
    if (arr[i] != v) { return false; }
  }
  return arr.size() > 0;
}

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
TTToe::TTToe(int p1v, int p2v) {
  _actors = {this->_nil, p1v, p2v};
  _GOALS= mapGoalSpace();
}

//////////////////////////////////////////////////////////////////////////////
//
bool TTToe::isNil(int cellv) {
  return cellv == _nil;
}

//////////////////////////////////////////////////////////////////////////////
// if brand new game, just make a random move
int TTToe::getFirstMove() {
  auto sz= _grid.size();
  return sz > 0 && every(_grid, _nil) ? cx::randInt(sz) : -1;
}

//////////////////////////////////////////////////////////////////////////////
//
void TTToe::syncState(const ArrCells &seed, int actor) {
  S__COPY(seed, _grid);
  _actors[0] = actor;
}

//////////////////////////////////////////////////////////////////////////////
// find set of empty slots
const s_vec<int>
TTToe::getNextMoves(not_null<ag::FFrame<BD_SZ>*> snap) {

  auto sz= snap->_state.size();
  s_vec<int> rc;

  for (auto pos= 0; pos < sz; ++pos) {
    if (isNil(snap->_state[pos])) {
      rc.push_back(pos);
    }
  }

  return rc;
}

//////////////////////////////////////////////////////////////////////////////
//
void TTToe::undoMove(not_null<ag::FFrame<BD_SZ>*> snap, int move) {
  assert(move >= 0 && move < snap->_state.size());
  snap->_state[move] = _nil;
}

//////////////////////////////////////////////////////////////////////////////
//
void TTToe::makeMove(not_null<ag::FFrame<BD_SZ>*> snap, int move) {
  assert(move >= 0 && move < snap->_state.size());
  if (isNil(snap->_state[move])) {
    snap->_state[move] = snap->_cur;
  } else {
    throw "cell [" + FTOS(move) + "] is not free";
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void TTToe::switchPlayer(not_null<ag::FFrame<BD_SZ>*> snap) {
  auto t = snap->_cur;
  snap->_cur= snap->_other;
  snap->_other=t;
}

//////////////////////////////////////////////////////////////////////////////
//
int TTToe::getOtherPlayer(int pv) {
  if (pv == _actors[1]) {
    return _actors[2];
  }

  if (pv == _actors[2]) {
    return _actors[1];
  }

  return _nil;
}

//////////////////////////////////////////////////////////////////////////////
//
owner<ag::FFrame<BD_SZ>*>  TTToe::takeFFrame() {
  auto ff = mc_new( ag::FFrame<BD_SZ> );

  ff->_other= getOtherPlayer(_actors[0]);
  ff->_cur= _actors[0];
  ff->_lastBestMove= -1;
  S__COPY(_grid, ff->_state);

  return ff;
}

//////////////////////////////////////////////////////////////////////////////
//
int TTToe::evalScore(not_null<ag::FFrame<BD_SZ>*> snap) {
  // if we lose, return a nega value
  F__LOOP(it, _GOALS) {
    auto &t= *it;
    if (testWin(snap->_state, snap->_other, t)) {
      return -100;
    }
  }
  return 0;
}

//////////////////////////////////////////////////////////////////////////////
//
bool TTToe::isOver(not_null<ag::FFrame<BD_SZ>*> snap) {

  F__LOOP(it, _GOALS) {
    auto &t = *it;
    if (testWin(snap->_state, snap->_other, t) ||
        testWin(snap->_state, snap->_cur, t)) {
      return true;
    }
  }
  return isStalemate(snap);
}

//////////////////////////////////////////////////////////////////////////////
//
bool TTToe::isStalemate(not_null<ag::FFrame<BD_SZ>*> snap) {
  return not_any(snap->_state, _nil);
}

//////////////////////////////////////////////////////////////////////////////
//
int TTToe::getWinner(not_null<ag::FFrame<BD_SZ>*> snap, ArrDim &combo) {

  int win= -1;
  F__LOOP(it, _GOALS) {
    auto &t= *it;
    if (testWin(snap->_state, snap->_other, t)) {
      win=snap->_other;
    }
    else
    if (testWin(snap->_state, snap->_cur, t)) {
      win=snap->_cur;
    }
    if (win > 0) {
      S__COPY(t,combo);
      break;
    }
  }
  return win;
}

//////////////////////////////////////////////////////////////////////////////
//
bool TTToe::testWin(const ArrCells &vs, int actor, const ArrDim &g) {

  auto cnt=g.size();
  for (auto n= 0; n < g.size(); ++n) {
    if (actor == vs[g[n]]) {
      --cnt;
    }
  }
  return cnt == 0;
}



NS_END


