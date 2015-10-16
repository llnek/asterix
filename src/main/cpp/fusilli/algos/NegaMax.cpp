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

#include "negamax.h"
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////
//
static const int PINF = 1000000;
static int negamax(GameBoard* board, game,
    int maxDepth, int depth, int alpha, int beta) {

  if (depth == 0 || board->IsOver(game)) {
    return board->EvalScore(game);
  }

  auto openMoves = board->GetNextMoves(game);
  auto bestMove = openMoves[0];
  auto bestValue = -PINF;
  int rc;
  int move;

  if (depth == maxDepth) {
    game->LastBestMove = openMoves[0];
  }

  for (auto n=0; n < openMoves.length; ++n) {
    move = openMoves[n];
    board->MakeMove(game, move);
    board->SwitchPlayer(game);
    rc = - negamax(board, game, maxDepth, depth-1, -beta, -alpha);
    board->SwitchPlayer(game);
    board->UnmakeMove(game,move);
    bestValue = ::max(bestValue,  rc );
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
bool GameBoard::IsOver(game) {
  return false;
}

//////////////////////////////////////////////////////////////////////////
//
int GameBoard::EvalScore(game) {
  return 0;
}

//////////////////////////////////////////////////////////////////////////
//
const s::vector<> GameBoard::GetNextMoves(game) {
  return s::vector<int>{};
}

//////////////////////////////////////////////////////////////////////////
//
void GameBoard::MakeMove(game, int move) {

}

//////////////////////////////////////////////////////////////////////////
//
void GameBoard::SwitchPlayer(game) {

}

//////////////////////////////////////////////////////////////////////////
//
void GameBoard::UndoMove(game, int move) {

}

//////////////////////////////////////////////////////////////////////////
//
Snapshot GameBoard::TakeSnapshot() {
  Snapshot s;
  return s;
}

//////////////////////////////////////////////////////////////////////////
//
int EvalNegaMax(not_null<GameBoard*> board) {
  auto snapshot= board->TakeSnapshot();
  negamax(board, snapshot, 10, 10, -PINF, PINF);
  return snapshot.lastBestMove;
}

NS_END(fusii)


