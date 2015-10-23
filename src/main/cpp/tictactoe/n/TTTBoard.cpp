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
#include "TTTBoard.h"
#include "s/utils.h"
#include <math.h>
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
static bool not_any(const s::array<int,BD_SZ * BD_SZ>& arr, int v) {
  for (int i=0; i < arr.size(); ++i) {
    if (arr[i] == v) { return false; }
  }
  return arr.size() > 0;
}

//////////////////////////////////////////////////////////////////////////////
//
static bool every(const s::array<int,BD_SZ * BD_SZ>& arr, int v) {
  for (int i=0; i < arr.size(); ++i) {
    if (arr[i] != v) { return false; }
  }
  return arr.size() > 0;
}


//////////////////////////////////////////////////////////////////////////
//
TTTBoard::TTTBoard(int nil, int p1v, int p2v) {
  this->actors = {nil, p1v, p2v};
  this->CV_Z= nil;
  this->GOALS= MapGoalSpace();
}

//////////////////////////////////////////////////////////////////////////////
//
TTTBoard::~TTTBoard() {
}

//////////////////////////////////////////////////////////////////////////
//
bool TTTBoard::IsNil(int cellv) {
  return cellv == this->CV_Z;
}

//////////////////////////////////////////////////////////////////////////
//
int TTTBoard::GetFirstMove() {
  auto sz= grid.size();
  bool virgo=true;

  for (int i=0; i < sz; ++i) {
    if (grid[i] != CV_Z) {
      virgo=false;
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
void TTTBoard::SyncState(const s::array<int,BD_SZ * BD_SZ>& seed, int actor) {
  s::copy(s::begin(seed), s::end(seed), s::begin(grid));
  actors[0] = actor;
}

//////////////////////////////////////////////////////////////////////////
//
const s::vector<int> TTTBoard::GetNextMoves(not_null<ag::Snapshot<BD_SZ>*> snap) {
  s::vector<int> rc;
  int pos=0;

  for (int pos= 0; pos < snap->state.size(); ++pos) {
    if (IsNil(snap->state[pos])) {
      rc.push_back(pos);
    }
    ++pos;
  }
  return rc;
}

//////////////////////////////////////////////////////////////////////////
//
void TTTBoard::UndoMove(not_null<ag::Snapshot<BD_SZ>*> snap, int move) {
  snap->state[move] = CV_Z;
}

//////////////////////////////////////////////////////////////////////////
//
void TTTBoard::MakeMove(not_null<ag::Snapshot<BD_SZ>*> snap, int move) {
  if (IsNil(snap->state[move])) {
    snap->state[move] = snap->cur;
  } else {
    throw "Fatal Error: cell [" + move + "] is not free";
  }
}

//////////////////////////////////////////////////////////////////////////
//
void TTTBoard::SwitchPlayer(not_null<ag::Snapshot<BD_SZ>*> snap) {
  auto t = snap->cur;
  snap->cur= snap->other;
  snap->other=t;
}

//////////////////////////////////////////////////////////////////////////
//
int TTTBoard::GetOtherPlayer(int pv) {
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
ag::Snapshot<BD_SZ> TTTBoard::TakeSnapshot() {
  ag::Snapshot<BD_SZ> s;

  s::copy(s::begin(grid), s::end(grid), s::begin(s.state));
  s.other= GetOtherPlayer(actors[0]);
  s.cur= actors[0];
  s.lastBestMove= -1;

  return s;
}

//////////////////////////////////////////////////////////////////////////
//
int TTTBoard::EvalScore(not_null<ag::Snapshot<BD_SZ>*> snap) {
  // if we lose, return a nega value
  s::array<int,BD_SZ> combo;
  return IsWinner(snap.get(), snap->other, combo) > 0 ? -100 : 0;
}

//////////////////////////////////////////////////////////////////////////
//
bool TTTBoard::IsOver(not_null<ag::Snapshot<BD_SZ>*> snap) {
  s::array<int,BD_SZ> combo;
  return IsWinner(snap.get(), snap->cur, combo) > 0 ||
         IsWinner(snap.get(), snap->other, combo) > 0 ||
         IsStalemate(snap.get());
}

//////////////////////////////////////////////////////////////////////////
//
bool TTTBoard::IsStalemate(not_null<ag::Snapshot<BD_SZ>*> snap) {
  //TODO: fix
  if (NNP(snap)) {
    return not_any(snap->state, CV_Z);
  } else {
    return not_any(grid, CV_Z);
  }
}

//////////////////////////////////////////////////////////////////////////
//
int TTTBoard::IsWinner(ag::Snapshot<BD_SZ>* snap, int actor,
    s::array<int,BD_SZ>& combo) {
  int win= -1;
  for (auto it = GOALS.begin(); it != GOALS.end(); ++it) {
    auto& t= *it;
    if (TestWin(snap,actor, *it)) {
      win=actor;
      s::copy(s::begin(t),s::end(t),s::begin(combo));
    }
  }
  return win;
}

//////////////////////////////////////////////////////////////////////////////
//
bool TTTBoard::TestWin(ag::Snapshot<BD_SZ>* snap, int actor,
    const s::array<int,BD_SZ>& g) {

  s::array<int,BD_SZ>& vs= ENP(snap) ? grid : snap->state;
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

