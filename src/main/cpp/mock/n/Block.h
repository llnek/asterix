// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "core/COMP.h"
NS_BEGIN(mock)

#define TILE_H_SIZE 6
#define TILE_W_SIZE 8

enum  {
  kBlockGap= 0,
  kBlock1,
  kBlock2,
  kBlock3,
  kBlock4
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Block : public c::Sprite {

  __decl_iz(_type)

public:

  void setupBlock(float w, float h, int type);
  static owner<Block*> create();

  __decl_gsms(int,_type,Type)
};


NS_END


