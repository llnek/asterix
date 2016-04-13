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
NS_BEGIN(hockey)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Mallet : public f::CPixie {
  static owner<Mallet*> create() {
    auto z= mc_new(Mallet);
    z->initWithSpriteFrameName("mallet.png");
    z->autorelease();
    return z;
  }
  __decl_ptr(c::Touch, tap)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Puck : public f::CPixie {
  static owner<Puck*> create() {
    auto z= mc_new(Puck);
    z->initWithSpriteFrameName("puck.png");
    z->autorelease();
    return z;
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Player : public f::CStats {
  Player(int n) : CStats(n) {}
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Players : public ecs::Component {
  __decl_comp_tpid("n/Players")
  __decl_arr(Player,3, parr)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {
  __decl_comp_tpid( "n/GVars" )
  __decl_fz(goalWidth)
  __decl_fz(sq_radii)
  __decl_md(CCT_PT,ballNextPos)
  __decl_md(CCT_PT,ballVec)
};


//////////////////////////////////////////////////////////////////////////////
//
void setPos(not_null<ecs::Node*>, float x, float y);
void readyPt(const s_vec<ecs::Node*>&, ecs::Node*);


NS_END




