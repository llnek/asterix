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
NS_BEGIN(monsters)

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL HUDLayer : public f::XLayer {

  __decl_create_layer(HUDLayer)
  __decl_deco_ui()
  __decl_get_iid(3)

  __decl_ptr(c::Label, _stateLabel)
  __decl_ptr(c::Label, _scoreLabel)
  __decl_iz(_score)

  s_arr<c::Label*,3> _coinLabels;
  s_arr<int,3> _coins;

  void updateCoins(int player, int score);
  void updateAIState(const sstr&);
  void updateScore(int);
  int getScore() { return _score; }

};


NS_END



