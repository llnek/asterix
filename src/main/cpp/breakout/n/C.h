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

NS_BEGIN(breakout)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Brick : public f::CPixie {
    Brick(int v, int color) {
    this->_color=color;
    this->_value=v;
  }
  __decl_bf(_flipped);
  __decl_iz(_color);
  __decl_iz(_value);

public:

  __decl_gsms_is(_flipped, Flipped)
  __decl_getr(int, _value, Value)

  static owner<Brick*> create(const sstr &n, int v, int c) {
    auto z= mc_new2(Brick, v, c);
    z->initWithSpriteFrameName(n);
    XCFG()->fit(z);
    z->autorelease();
    return z;
  }


};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL BrickFence : public ecs::Component {
  __decl_comp_tpid("n/BrickFence")
  __decl_vec(Brick*, bricks)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {
  __decl_comp_tpid("n/GVars")

};

NS_END


