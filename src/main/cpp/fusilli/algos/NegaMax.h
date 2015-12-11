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
NS_BEGIN(fusii)
NS_BEGIN(algos)

const int PINF = 1000000;

//////////////////////////////////////////////////////////////////////////////
//
template<int Z> struct FS_DLL FFrame {
  s_arr<int, Z*Z> state;
  int lastBestMove=0;
  int other=0;
  int cur=0;
};

//////////////////////////////////////////////////////////////////////////////
template<int Z> class FS_DLL GameBoard {
public:

  virtual const s_vec<int> getNextMoves(not_null<FFrame<Z>*>) = 0;
  virtual int evalScore(not_null<FFrame<Z>*>) = 0;

  virtual bool isStalemate(not_null<FFrame<Z>*>) = 0;
  virtual bool isOver(not_null<FFrame<Z>*>) = 0;

  virtual void undoMove(not_null<FFrame<Z>*>, int move) = 0;
  virtual void makeMove(not_null<FFrame<Z>*>, int move) = 0;

  virtual void switchPlayer(not_null<FFrame<Z>*>) = 0;
  virtual owner<FFrame<Z>*> takeFFrame() = 0;

  virtual ~GameBoard() {}
};

BEGIN_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
template <int Z> int negaMax(not_null<GameBoard<Z>*> board,
    not_null<FFrame<Z>*> game,
    int maxDepth, int depth, int alpha, int beta) {

  if (depth == 0 || board->isOver(game)) {
    return board->evalScore(game);
  }

  auto openMoves = board->getNextMoves(game);
  auto bestMove = openMoves[0];
  int bestValue = -PINF;

  if (depth == maxDepth) {
    game->lastBestMove = openMoves[0];
  }

  F__LOOP(it, openMoves) {
    int move = *it;
    int rc;
    //try  a move
    board->makeMove(game, move);
    board->switchPlayer(game);
    rc = - negaMax(board, game, maxDepth, depth-1, -beta, -alpha);
    //now, roll it back
    board->switchPlayer(game);
    board->undoMove(game, move);
    //how did we do ?
    bestValue = s::max(bestValue,  rc);
    if (alpha < rc) {
      alpha = rc;
      bestMove = move;
      if (depth == maxDepth) {
        game->lastBestMove = move;
      }
      if (alpha >= beta) { break; }
    }
  }

  return bestValue;
}

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////
// Main method for nega-max algo
template <int Z> int evalNegaMax(not_null<GameBoard<Z>*> board) {
  auto f= board->takeFFrame();
  negaMax<Z>(board, f, 10, 10, -PINF, PINF);
  auto rc = f->lastBestMove;
  delete f;
  return rc;
}


NS_END(algos)
NS_END(fusii)
#endif

