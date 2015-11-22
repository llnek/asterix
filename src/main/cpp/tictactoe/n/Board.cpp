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
#include "Board.h"
#include "s/utils.h"
#include <math.h>
NS_BEGIN(tttoe)

BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
bool not_any(const ArrCells& arr, int v) {
  for (int i=0; i < arr.size(); ++i) {
    if (arr[i] == v) { return false; }
  }
  return arr.size() > 0;
}

//////////////////////////////////////////////////////////////////////////////
//
bool every(const ArrCells& arr, int v) {
  for (int i=0; i < arr.size(); ++i) {
    if (arr[i] != v) { return false; }
  }
  return arr.size() > 0;
}
END_NS_UNAMED()

//////////////////////////////////////////////////////////////////////////
//
Board::Board(int nil, int p1v, int p2v) {
  this->actors = {nil, p1v, p2v};
  this->GOALS= mapGoalSpace();
  this->CV_Z= nil;
}

//////////////////////////////////////////////////////////////////////////////
//
Board::~Board() {
}

//////////////////////////////////////////////////////////////////////////
//
bool Board::isNil(int cellv) {
  return cellv == this->CV_Z;
}

//////////////////////////////////////////////////////////////////////////
//
int Board::getFirstMove() {
  auto sz= grid.size();
  bool virgo=true;

  for (int i=0; i < sz; ++i) {
    if (grid[i] != CV_Z) {
      virgo=false;
      break;
    }
  }

  if (virgo) {
    return (int)
      ::floor(c::RandomHelper::random_real<float>(0.0f, (float)sz));
  } else {
    return -1;
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Board::syncState(const ArrCells& seed, int actor) {
  s::copy(s::begin(seed), s::end(seed), s::begin(grid));
  actors[0] = actor;
}

//////////////////////////////////////////////////////////////////////////
//
const s::vector<int>
Board::getNextMoves(not_null<ag::FFrame<BD_SZ>*> snap) {

  s::vector<int> rc;

  for (int pos= 0; pos < snap->state.size(); ++pos) {
    if (isNil(snap->state[pos])) {
      rc.push_back(pos);
    }
  }

  return rc;
}

//////////////////////////////////////////////////////////////////////////
//
void Board::undoMove(not_null<ag::FFrame<BD_SZ>*> snap, int move) {
  assert(move >= 0 && snap->state.size());
  snap->state[move] = CV_Z;
}

//////////////////////////////////////////////////////////////////////////
//
void Board::makeMove(not_null<ag::FFrame<BD_SZ>*> snap, int move) {
  assert(move >= 0 && snap->state.size());
  if (isNil(snap->state[move])) {
    snap->state[move] = snap->cur;
  } else {
    throw "Fatal Error: cell [" + move + "] is not free";
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Board::switchPlayer(not_null<ag::FFrame<BD_SZ>*> snap) {
  auto t = snap->cur;
  snap->cur= snap->other;
  snap->other=t;
}

//////////////////////////////////////////////////////////////////////////
//
int Board::getOtherPlayer(int pv) {
  if (pv == actors[1]) {
    return actors[2];
  }
  else
  if (pv == actors[2]) {
    return actors[1];
  }
  else {
    return CV_Z;
  }
}

//////////////////////////////////////////////////////////////////////////
//
ag::FFrame<BD_SZ> Board::takeFFrame() {
  ag::FFrame<BD_SZ> ff;

  s::copy(s::begin(grid), s::end(grid), s::begin(ff.state));
  ff.other= getOtherPlayer(actors[0]);
  ff.cur= actors[0];
  ff.lastBestMove= -1;

  return ff;
}

//////////////////////////////////////////////////////////////////////////
//
int Board::evalScore(not_null<ag::FFrame<BD_SZ>*> snap) {
  // if we lose, return a nega value
  ArrDim  combo;
  return testWin(snap->state, snap->other, combo) ? -100 : 0;
}

//////////////////////////////////////////////////////////////////////////
//
bool Board::isOver(not_null<ag::FFrame<BD_SZ>*> snap) {
  ArrDim combo;
  return testWin(snap->state, snap->other, combo) ||
         testWin(snap->state, snap->cur, combo) ||
         isStalemate(snap);
}

//////////////////////////////////////////////////////////////////////////
//
bool Board::isStalemate(not_null<ag::FFrame<BD_SZ>*> snap) {
  return not_any(snap->state, CV_Z);
  //return not_any(grid, CV_Z);
}

//////////////////////////////////////////////////////////////////////////
//
int Board::getWinner(not_null<ag::FFrame<BD_SZ>*> snap, ArrDim& combo) {

  int win= -1;
  F__LOOP(it, GOALS) {
    auto& t= *it;
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
bool Board::testWin(const ArrCells& vs, int actor, const s::array<int,BD_SZ>& g) {

  int cnt=0;
  for (int n= 0; n < g.size(); ++n) {
    auto pos= g[n];
    if (actor == vs[pos]) {
      ++cnt;
    }
  }
  return cnt == g.size();
}




NS_END(tttoe)

