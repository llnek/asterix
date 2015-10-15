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

#if !defined(__TTTBOARD_H__)
#define __TTTBOARD_H__

#include "algos/NegaMax.h"
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
// A Tic Tac Toe board
class FS_DLL TTTBoard : public GameBoard {
public:

  bool IsNil(int cellv);

  TTTBoard(int size, int nil,  int p1v,  int p2v, int goals);

  int GetFirstMove();

  void SyncState(int seed, int actor);

  virtual GetNextMoves(Snapshot&);

  virtual void UndoMove(Snapshot&, int move);

  virtual void MakeMove(Snapshot& snap, int move);

  virtual void SwitchPlayer(Snapshot&);

  int getOtherPlayer(int pv);

  virtual Snapshot TakeSnapshot();

  virtual int EvalScore(Snapshot&);

  virtual bool IsOver(Snapshot&);

  virtual bool IsStalemate(Snapshot&);

  virtual bool IsWinner(int actor, Snapshot& );

};

NS_END(tttoe)
#endif
