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

#if !defined(__NEGAMAX_H__)
#define __NEGAMAX_H__

#include "aeon/fusilli.h"
NS_ALIAS(s,std)
NS_BEGIN(negamax)

//////////////////////////////////////////////////////////////////////////////
class FS_DLL GameBoard {
public:

  virtual const s::vector<int> GetNextMoves(game);
  virtual int EvalScore(Snapshot& game);
  virtual bool IsStalemate();
  virtual bool IsOver(game);
  virtual Snapshot TakeSnapshot();

  virtual void MakeMove(Snapshot&, int move);
  virtual void SwitchPlayer(game);
  virtual void UndoMove(game, int move);

  IMPL_CTOR(GameBoard)
};

//////////////////////////////////////////////////////////////////////////
//
struct Snapshot {
  int lastBestMove;
  other;
  cur;
  state;
};


NS_END(negamax)
#endif

