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
#include "cobjs.h"
#include <tuple>
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
// A Tic Tac Toe board
class FS_DLL TTTBoard : public f::GameBoard<TTT_SIZE> {
protected:
  const s::vector<s::array<int,TTT_SIZE>>& GOALS;
  s::array<int,TTT_CELLS> grid;
  s::array<int,3> actors;
  int CV_Z;
public:

  TTTBoard( int nil,  int p1v,  int p2v,
      const s::vector<s::array<int,TTT_SIZE>>& goals);

  virtual ~TTTBoard();

  virtual bool IsNil(int cellv);

  virtual int GetFirstMove();

  virtual void SyncState(const s::array<int,TTT_CELLS>& seed, int actor);

  virtual const s::vector<int> GetNextMoves(not_null<f::Snapshot<TTT_SIZE>*>);

  virtual void UndoMove(not_null<f::Snapshot<TTT_SIZE>*>, int move);

  virtual void MakeMove(not_null<f::Snapshot<TTT_SIZE>*>, int move);

  virtual void SwitchPlayer(not_null<f::Snapshot<TTT_SIZE>*>);

  virtual int GetOtherPlayer(int pv);

  virtual f::Snapshot<TTT_SIZE> TakeSnapshot();

  virtual int EvalScore(not_null<f::Snapshot<TTT_SIZE>*>);

  virtual bool IsOver(not_null<f::Snapshot<TTT_SIZE>*>);

  virtual bool IsStalemate(f::Snapshot<TTT_SIZE>*);

  virtual int IsWinner(
      f::Snapshot<TTT_SIZE>*, int actor, s::array<int,TTT_SIZE>& combo);
  bool TTTBoard::TestWin(f::Snapshot<TTT_SIZE>* snap, int actor,
      const s::array<int,TTT_SIZE>& g) ;

};


NS_END(tttoe)
#endif
