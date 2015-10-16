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

#include "TTTBoard.h"
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////
//
bool TTTBoard::IsNil(int cellv) {
  return cellv == CV_Z;
}

//////////////////////////////////////////////////////////////////////////
//
TTTBoard::TTTBoard(size_t size, int nil, int p1v, int p2v, void* goals) {
  this->actors= s::array<int,3>{ nil, p1v, p2v};
  this->CV_Z= nil;
  this->grid= new f::FArray<int>(size);
  this->size= size;
  this->GOALSPACE=goals;
}

//////////////////////////////////////////////////////////////////////////////
//
TTTBoard::~TTTBoard() {
  mc_del_arr(grid)
}

//////////////////////////////////////////////////////////////////////////
//
int TTTBoard::GetFirstMove() {
  return grid->All(nil) ? grid->RandomIndex() : -1;
}

//////////////////////////////////////////////////////////////////////////
//
void TTTBoard::SyncState(const f::FArray<int>& seed, int actor) {
  grid->Clone(seed);
  actors[0] = actor;
}

//////////////////////////////////////////////////////////////////////////
//
const s::vector<int> TTTBoard::GetNextMoves(Snapshot& snap) {
  s::vector<int> rc;
  int pos=0;
  for (int pos= 0; pos < snap.state.Size(); ++pos) {
    if (IsNil(snap.state[pos])) {
      rc.push_back(pos);
    }
    ++pos;
  }
  return rc;
}

//////////////////////////////////////////////////////////////////////////
//
void TTTBoard::UndoMove(Snapshot& snap, int move) {
  snap.state.Set(move, CV_Z);
}

//////////////////////////////////////////////////////////////////////////
//
void TTTBoard::MakeMove(Snapshot& snap, int move) {
  if (IsNil(snap.state[move])) {
    snap.state.Set(move, snap.cur);
  } else {
    throw "Fatal Error: cell [" + move + "] is not free";
  }
}

//////////////////////////////////////////////////////////////////////////
//
void TTTBoard::SwitchPlayer(Snapshot& snap) {
  auto t = snap.cur;
  snap.cur= snap.other;
  snap.other=t;
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
Snapshot TTTBoard::TakeSnapshot() {
  Snapshot s;
  s.other= GetOtherPlayer(actors[0]);
  s.cur= actors[0];
  s.state= *grid;
  s.lastBestMove= -1;
  return s;
}

//////////////////////////////////////////////////////////////////////////
//
int TTTBoard::EvalScore(Snapshot& snap) {
  // if we lose, return a nega value
  /*
  return sjs.isnum(this.isWinner(snap.other,
                                    snap.state )[0]) ? -100 : 0;
                                    */
  return 0;
}

//////////////////////////////////////////////////////////////////////////
//
bool TTTBoard::IsOver(not_null<Snapshot*> snap) {
  return sjs.isnum(IsWinner(snap->cur, snap.get())[0]) ||
         sjs.isnum(IsWinner(snap->other, snap.get())[0]) ||
         IsStalemate(snap.get());
}

//////////////////////////////////////////////////////////////////////////
//
bool TTTBoard::IsStalemate(Snapshot* snap) {
  if (NNP(snap)) {
    return snap->state.NotAny(nil);
  } else {
    return grid->NotAny(nil);
  }
}

//////////////////////////////////////////////////////////////////////////
//
s::tuple<int,f::FArray<int>>
TTTBoard::IsWinner(int actor, Snapshot* snap) {
  f::FArray<int> combo(this->size);
  int win= -1;
  for (auto it = GOALS.begin(); it != GOALS.end(); ++it) {
    if (TestWin(*it, actor, snap)) {
      win=actor;
      combo.Clone(*it);
    }
  }
  return s::make_tuple(win, combo);
}

//////////////////////////////////////////////////////////////////////////////
//
bool TTTBoard::TestWin(const FArray<int>& g, int actor, Snapshot* snap) {
  auto vs= ENP(snap) ? this->grid : &snap->state;
  int cnt=0;
  for (int n= 0; n < g.Size(); ++n) {
    if (actor == vs->Get(g[n])) {
      ++cnt;
    }
  }
  return cnt == g.Size();
}




NS_END(tttoe)

