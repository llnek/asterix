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

#if !defined(__HUD_H_)
#define __HUD_H_

#include "x2d/XLayer.h"
#include "n/CObjs.h"

NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL HUDLayer : public f::XLayer {

  STATIC_REIFY_LAYER(HUDLayer)
  virtual ~HUDLayer() {}
  HUDLayer() {}
  NOCPYASS(HUDLayer)

  virtual int getIID() { return 3; }
  virtual void decorate();

  void regoPlayers(const Player&, const Player&);
  void updateScore(const sstr&, int,  int score);
  void updateScores(j::json);
  void resetAsNew();
  void reset();
  void endGame();
  int isDone();
  void drawScores();

  //void drawResult(const sstr &winner);
  void drawResult(int winner);

  s_arr<Player,3> parr;
  s_arr<int,3> scores;
  DECL_PTR(c::Label, score2)
  DECL_PTR(c::Label, score1)
  DECL_PTR(c::Label, title)
  DECL_PTR(c::Label, resultMsg)

};


NS_END(pong)
#endif


