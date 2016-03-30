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

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(hockey)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Mallet : public f::CPixie {
  Mallet(not_null<c::Node*> s)
  : CPixie(s) {
  }
  __decl_ptr(c::Touch, tap)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Puck : public f::CPixie {
  Puck(not_null<c::Node*> s)
  : CPixie(s) {
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Player : public f::CStats {
  MDECL_COMP_TPID("n/Player")
  Player(int n) : CStats(n) {}
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Players : public ecs::Component {
  MDECL_COMP_TPID("n/Players")
  s_arr<Player,3> parr;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {
  MDECL_COMP_TPID( "n/GVars" )
  __decl_fz(goalWidth)
  __decl_fz(sq_radii)
  __decl_td(c::Vec2,ballNextPos)
  __decl_td(c::Vec2,ballVec)
};


class Node;
void setPos(not_null<ecs::Node*>, float x, float y);

NS_END




