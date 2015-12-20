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

NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL HUDLayer : public f::XLayer {

  STATIC_REIFY_LAYER(HUDLayer)

  c::Label *scoreLabel=nullptr;
  c::Label *status=nullptr;
  int score=0;

  virtual int getIID() { return 3; }
  virtual void decorate();

  void drawStatusText(const sstr&);
  void endGame();
  void resetAsNew();
  void reset();
  void updateScore(int );

  virtual ~HUDLayer() {}
  HUDLayer() {}

  NOCPYASS(HUDLayer)

};


NS_END(tetris)
#endif



