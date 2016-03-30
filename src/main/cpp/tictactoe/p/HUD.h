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
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL HUDLayer : public f::XLayer {

  STATIC_REIFY_LAYER(HUDLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(3)

  __decl_ptr(c::Label, _countDown)
  __decl_ptr(c::Label, _result)
  __decl_ptr(c::Label, _status)
  __decl_ptr(c::Label, _title)
  __decl_ptr(c::Label, _score1)
  __decl_ptr(c::Label, _score2)
  __decl_bf(_countDownState)
  __decl_iz(_countDownValue)

  s_arr<int, 3> _scores;

  __decl_td(sstr, _p2Long)
  __decl_td(sstr, _p1Long)
  __decl_td(sstr, _play2)
  __decl_td(sstr, _play1)
  __decl_td(sstr, _p2ID)
  __decl_td(sstr, _p1ID)

  void regoPlayers(const sstr &color1,
                   const sstr &p1k, const sstr &p1n,
                   const sstr &color2,
                   const sstr &p2k, const sstr &p2n);

  void resetAsNew();
  void reset();

  void draw(bool running,  int category, int pnum);
  void drawXXXText(c::Label*, const sstr&);
  void drawStatus(int category, int pnum);
  void updateScore(int pnum, int value);
  void showCountDown(const sstr&);
  void drawResult(int pnum);
  void endGame(int winner);
  void updateTimer(float);
  void drawScores();
  void showTimer();
  void killTimer();

};


NS_END


