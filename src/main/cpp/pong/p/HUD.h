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

NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL HUDLayer : public f::XLayer {

  STATIC_REIFY_LAYER(HUDLayer)
  virtual ~HUDLayer() {}
  HUDLayer() {}
  NOCPYASS(HUDLayer)

  virtual void decorate();

  void regoPlayers(p1,p1ids,p2,p2ids);
  void resetAsNew();
  void reset();
  void endGame();
  bool isDone();
  void updateScores(scores);
  void updateScore(color,value);
  void drawScores();
  void drawResult(winner);

  s_arr<sstr,3> scores;
  sstr p1Color;
  sstr p2Color;
  sstr p2Long;
  sstr p1Long;
  sstr p2ID;
  sstr p1ID;

};


NS_END(pong)
#endif


