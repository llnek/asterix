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

#include "x2d/XLayer.h"
#include "n/C.h"

NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL HUDLayer : public f::XLayer {

  void regoPlayers(const Player&, const Player&);
  void updateScore(const sstr&, int,  int score);
  void drawResult(int winner);
  void drawScores();
  void updateScores(j::json);
  void endGame();
  int isDone();

  DECL_PTR(c::Label, resultMsg)
  DECL_PTR(c::Label, score2)
  DECL_PTR(c::Label, score1)
  DECL_PTR(c::Label, title)
  s_arr<Player,3> parr;
  s_arr<int,3> scores;

  STATIC_REIFY_LAYER(HUDLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(3)

};


NS_END


