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

#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"
#include "GameSprite.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(mock)

enum  {
    kBlockGap,
    kBlock1,
    kBlock2,
    kBlock3,
    kBlock4
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Block : public GameSprite {

  DECL_IZ(_tileHeight)
  DECL_IZ(_tileWidth)
  DECL_IZ(_type)

public:

  static owner<Block*> create(const c::Rect &frame);
  void setupBlock(int width, int height, int type);

  DECL_MTDS(int,_type,Type);

  virtual int left() {
    return this->getPositionX();
  }

  virtual int right() {
    return this->getPositionX() + _width;
  }

  virtual int top() {
      return height();
  }

  virtual int bottom() {
    return 0;
  }


};

NS_END



