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
class CC_DLL HUDLayer : public f::XLayer {

  __decl_ptr(c::Label, _resultMsg)
  __decl_ptr(c::Label, _score2)
  __decl_ptr(c::Label, _score1)
  __decl_ptr(c::Label, _title)
  s_arr<Player,3> _parr;
  s_arr<int,3> _scores;

public:

  void regoPlayers(const Player&, const Player&);
  void updateScore(const sstr&, int,  int score);
  void drawResult(int winner);
  void drawScores();
  void updateScores(j::json);
  void endGame();
  int isDone();

  STATIC_REIFY_LAYER(HUDLayer)
  __decl_deco_ui()
  __decl_get_iid(3)

};


NS_END


