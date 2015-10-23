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
#include <algorithm>
#include <array>

NS_ALIAS(s,std)
NS_BEGIN(fusii)
NS_BEGIN(algos)

const int PINF = 1000000;

//////////////////////////////////////////////////////////////////////////
//
template<int Z>
struct FS_DLL Snapshot {
  s::array<int, Z*Z> state;
  int lastBestMove;
  int other;
  int cur;
};

//////////////////////////////////////////////////////////////////////////////
template<int Z>
class FS_DLL GameBoard {
protected:
  GameBoard() {}
public:

  virtual const s::vector<int> GetNextMoves(not_null<Snapshot<Z>*>) = 0;
  virtual int EvalScore(not_null<Snapshot<Z>*>) = 0;

  virtual bool IsStalemate(not_null<Snapshot<Z>*>) = 0;
  virtual bool IsOver(not_null<Snapshot<Z>*>) = 0;

  virtual void UndoMove(not_null<Snapshot<Z>*>, int move) = 0;
  virtual void MakeMove(not_null<Snapshot<Z>*>, int move) = 0;

  virtual void SwitchPlayer(not_null<Snapshot<Z>*>) = 0;
  virtual Snapshot<Z> TakeSnapshot() = 0;

  NO__CPYASS(GameBoard)
  virtual ~GameBoard() {}

};

//////////////////////////////////////////////////////////////////////////////
//
template <int Z>
int NegaMax(not_null<GameBoard<Z>*> board,
    not_null<Snapshot<Z>*> game,
    int maxDepth, int depth, int alpha, int beta) {

  if (depth == 0 || board->IsOver(game)) {
    return board->EvalScore(game);
  }

  auto openMoves = board->GetNextMoves(game);
  auto bestMove = openMoves[0];
  int bestValue = -PINF;

  if (depth == maxDepth) {
    game->LastBestMove = openMoves[0];
  }

  for (auto it=openMoves.begin(); it != openMoves.end(); ++it) {
    int move = *it;
    int rc;
    board->MakeMove(game, move);
    board->SwitchPlayer(game);
    rc = - NegaMax(board, game, maxDepth, depth-1, -beta, -alpha);
    board->SwitchPlayer(game);
    board->UndoMove(game, move);
    bestValue = s::max(bestValue,  rc);
    if (alpha < rc) {
      alpha = rc;
      bestMove = move;
      if (depth == maxDepth) {
        game->LastBestMove = move;
      }
      if (alpha >= beta) { break; }
    }
  }

  return bestValue;
}

//////////////////////////////////////////////////////////////////////////
//
template <int Z>
int EvalNegaMax(not_null<GameBoard<Z>*> board) {
  auto snapshot= board->TakeSnapshot();
  NegaMax(board, snapshot, 10, 10, -PINF, PINF);
  return snapshot.lastBestMove;
}

NS_END(algos)
NS_END(fusii)
#endif

