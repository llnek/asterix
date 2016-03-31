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

#pragma once

//////////////////////////////////////////////////////////////////////////////
//
#include "algos/NegaMax.h"
#include "ecs/Ecs.h"
#include "lib.h"

NS_ALIAS(ag, fusii::algos)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
// A Tic Tac Toe board
class CC_DLL TTToe : public ag::GameBoard<BD_SZ>, public f::CAutoma {

  bool testWin(const ArrCells& , int actor, const ArrDim&);

  __decl_md(ArrCells, _grid)
  __decl_iz(_nil)

  s_vec<ArrDim> _GOALS;
  s_arr<int,3> _actors;

public:

  virtual const s_vec<int> getNextMoves(not_null<ag::FFrame<BD_SZ>*>);
  virtual int evalScore(not_null<ag::FFrame<BD_SZ>*>);

  virtual bool isStalemate(not_null<ag::FFrame<BD_SZ>*>);
  virtual bool isOver(not_null<ag::FFrame<BD_SZ>*>);

  virtual void undoMove(not_null<ag::FFrame<BD_SZ>*>, int move);
  virtual void makeMove(not_null<ag::FFrame<BD_SZ>*>, int move);

  virtual void switchPlayer(not_null<ag::FFrame<BD_SZ>*>);
  virtual owner<ag::FFrame<BD_SZ>*> takeFFrame();

  virtual int getWinner(not_null<ag::FFrame<BD_SZ>*>, ArrDim &combo);
  virtual void syncState(const ArrCells &seed, int actor);

  TTToe(int p1v,  int p2v);
  virtual ~TTToe() {}

  virtual int getOtherPlayer(int pv);
  virtual bool isNil(int cellv);
  virtual int getFirstMove();

  __decl_comp_tpid( "n/SmartAI" )

};


NS_END


