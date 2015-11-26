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

#if !defined(__BOARD_H__)
#define __BOARD_H__

#include "algos/NegaMax.h"
#include "s/utils.h"
#include <tuple>
NS_ALIAS(ag, fusii::algos)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
// A Tic Tac Toe board
class FS_DLL Board : public ag::GameBoard<BD_SZ> {
protected:

  bool testWin(const ArrCells& , int actor, const ArrDim& combo);

  s::vector<ArrDim> GOALS;
  s::array<int,3> actors;
  ArrCells grid;
  int CV_Z;

  NO__CPYASS(Board)
  Board() = delete;

public:

  Board(int nil,  int p1v,  int p2v);
  virtual ~Board();

  int getOtherPlayer(int pv);
  bool isNil(int cellv);
  int getFirstMove();

  int getWinner(not_null<ag::FFrame<BD_SZ>*>, ArrDim& combo);
  void syncState(const ArrCells& seed, int actor);

////
//
//
  virtual const s::vector<int> getNextMoves(not_null<ag::FFrame<BD_SZ>*>);
  virtual int evalScore(not_null<ag::FFrame<BD_SZ>*>);

  virtual bool isStalemate(not_null<ag::FFrame<BD_SZ>*>);
  virtual bool isOver(not_null<ag::FFrame<BD_SZ>*>);

  virtual void undoMove(not_null<ag::FFrame<BD_SZ>*>, int move);
  virtual void makeMove(not_null<ag::FFrame<BD_SZ>*>, int move);

  virtual void switchPlayer(not_null<ag::FFrame<BD_SZ>*>);
  virtual owner<ag::FFrame<BD_SZ>*> takeFFrame();

};


NS_END(tttoe)
#endif
