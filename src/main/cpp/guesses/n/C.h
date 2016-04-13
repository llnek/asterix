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

#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(guesses)


//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL MemoryTile : public f::CPixie {
  static owner<MemoryTile*> create(int v) {
    auto z= mc_new(MemoryTile);
    z->initWithSpriteFrameName("cover.png");
    z->autorelease();
    z->pvalue=v;
    return z;
  }
  __decl_bf(inflated)
  __decl_iz(pvalue)
};


//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {
  __decl_comp_tpid( "n/GVars" )
  __decl_vec(MemoryTile*, pickedTiles)
  __decl_vec(MemoryTile*, tiles)
  __decl_ptr(c::DelayTime,checkTimer)
  __decl_iz(moves)
};



NS_END




