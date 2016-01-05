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

#if !defined(__HUD_H__)
#define __HUD_H__

#include "x2d/XLayer.h"
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL HUDLayer : public f::XLayer {

  STATIC_REIFY_LAYER(HUDLayer)
  MDECL_DECORATE()
  MDECL_GET_IID(3)

  DECL_PTR(c::Label, countDown)
  DECL_PTR(c::Label, result)
  DECL_PTR(c::Label, status)
  DECL_PTR(c::Label, title)
  DECL_PTR(c::Label, score1)
  DECL_PTR(c::Label, score2)
  DECL_BF(countDownState)
  DECL_IZ(countDownValue)

  s_arr<int, 3> scores;

  DECL_TD(sstr, p2Long)
  DECL_TD(sstr, p1Long)
  DECL_TD(sstr, play2)
  DECL_TD(sstr, play1)
  DECL_TD(sstr, p2ID)
  DECL_TD(sstr, p1ID)

  void regoPlayers(const sstr &color1,
                   const sstr &p1k, const sstr &p1n,
                   const sstr &color2,
                   const sstr &p2k, const sstr &p2n);

  void showMenu(c::Ref*);
  void resetAsNew();
  void reset();

  void drawXXXText(c::Label*, const sstr&);
  void updateScore(int pnum, int value);
  void draw(bool running,  int pnum);
  void showCountDown(const sstr&);
  void drawResult(int pnum);
  void drawStatus(int pnum);
  void endGame(int winner);
  void updateTimer(float);
  void drawScores();
  void showTimer();
  void killTimer();

};


NS_END(tttoe)
#endif


