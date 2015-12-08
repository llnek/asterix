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

#include "x2d/XLives.h"
#include "x2d/XLayer.h"
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL HUDLayer : public f::XLayer {
protected:

  c::Label* scoreLabel;
  f::XLives* lives;
  int score;

public:

  STATIC_REIFY_LAYER(HUDLayer)
  NOCPYASS(HUDLayer)
  IMPLCZ(HUDLayer)

  void resetAsNew();
  void reset();

  virtual void decorate();
};


NS_END(terra)
#endif


