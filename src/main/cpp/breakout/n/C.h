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

#include "core/COMP.h"

NS_BEGIN(breakout)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Brick : public f::CPixie, public f::CStats {
  Brick(not_null<c::Node*> s, int v, int color)
  : CPixie(s) {
    this->color=color;
    this->value=v;
  }
  DECL_BF(flipped);
  DECL_IZ(color);
  MDECL_COMP_TPID("n/Brick")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL BrickFence : public ecs::Component {
  MDECL_COMP_TPID("n/BrickFence")
  s_vec<Brick*> bricks;
  virtual ~BrickFence() {
    F__LOOP(it,bricks)
    { delete *it; };
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {
  MDECL_COMP_TPID("n/GVars")

};

NS_END


