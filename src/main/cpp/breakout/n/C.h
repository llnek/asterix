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

#include "core/ComObj.h"

NS_BEGIN(breakout)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Ball : public f::ComObj {
  Ball(not_null<c::Sprite*> s) : ComObj(s) {}
  MDECL_COMP_TPID("n/Ball")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Brick : public f::ComObj {
  Brick(not_null<c::Sprite*> s, int value, int color)
  : ComObj(s,1,value) {
    this->color=color;
  }
  DECL_IZ(color);
  MDECL_COMP_TPID("n/Brick")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL BrickFence : public a::Component {
  MDECL_COMP_TPID("n/BrickFence")
  s_vec<Brick*> bricks;
  virtual ~BrickFence() {
      F__LOOP(it,bricks) { delete *it; };
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Gesture : public a::Component {
  MDECL_COMP_TPID("n/Gesture")
  DECL_BF(right)
  DECL_BF(left)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Paddle : public f::ComObj {
  Paddle(not_null<c::Sprite*> s)
  : ComObj(s) {
  }
  MDECL_COMP_TPID("n/Paddle")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public a::Component {
  MDECL_COMP_TPID("n/GVars")

};

NS_END(breakout)


