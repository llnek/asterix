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
TTTBoard::TTTBoard(int size, int nil, int p1v, int p2v, void* goals) {
  this->actors= s::array<int,3>{ nil, p1v, p2v};
  this->CV_Z= nil;
  //this->grid= [];
  this->size= size;
  this->GOALSPACE=goals;
}

//////////////////////////////////////////////////////////////////////////
//
int TTTBoard::GetFirstMove() {
  /*
  const rc= R.find((v) => {
    return v !== this.CV_Z;
  }, this.grid );
  if (! sjs.echt(rc)) {
    return sjs.rand(this.grid.length);
  } else {
    return undef;
  }
  */
  return -1;
}

//////////////////////////////////////////////////////////////////////////
//
void TTTBoard::SyncState(seed, int actor) {
  //this.grid=seed.slice(0);
  actors[0] = actor;
}

//////////////////////////////////////////////////////////////////////////
//
const s::vector<int> TTTBoard::GetNextMoves(Snapshot& snap) {
  s::vector<int> rc;
  int pos=0;
  for (auto it= snap.state.begin(); it != snap.state.end(); ++it) {
    if (IsNil(*it)) {
      rc.push_back(pos);
    }
    ++pos;
  }
  return rc;
}

//////////////////////////////////////////////////////////////////////////
//
void TTTBoard::UndoMove(Snapshot& snap, int move) {
  snap.state[move] = CV_Z;
}

//////////////////////////////////////////////////////////////////////////
//
void TTTBoard::MakeMove(Snapshot& snap, int move) {
  if (IsNil(snap.state[move])) {
    snap.state[move] = snap.cur;
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
  s.state= this.grid.slice(0),
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
bool TTTBoard::IsOver(Snapshot& snap) {
  /*
  return sjs.isnum(this.isWinner(snap.cur, snap.state)[0]) ||
         sjs.isnum(this.isWinner(snap.other, snap.state)[0]) ||
         this.isStalemate(snap.state);
         */
  return false;
}

//////////////////////////////////////////////////////////////////////////
//
bool TTTBoard::IsStalemate(Snapshot& snap) {
  return ! R.any((n) => {
    return n === this.CV_Z;
  },
  game || this.grid);
}

//////////////////////////////////////////////////////////////////////////
//
bool TTTBoard::IsWinner(int actor, Snapshot* snap) {
  let game= gameVals || this.grid,
  combo,
  rc= R.any((r) => {
    combo=r;
    return R.all((n) => { return n === actor; },
                 R.map((i) => { return game[i]; }, r) );
  }, this.GOALSPACE);
  return rc ? [actor, combo] : [null, null];
}


NS_END(tttoe)

