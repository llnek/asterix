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
struct CC_DLL Widget : public f::CPixie {
  virtual void setPos(float x, float y) {
    auto pos= c::Vec2(x,y);
    if (!nextPos.equals(pos)) {
      nextPos = pos;
    }
    f::CPixie::setPos(x,y);
  }
  void syncPos() {
    f::CPixie::setPos(nextPos.x,nextPos.y);
  }

  Widget(not_null<c::Node*> s)
  : CPixie(s) {}

  DECL_TD(c::Vec2,nextPos)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Mallet : public Widget {
  Mallet(not_null<c::Node*> s, int pnum)
    : Widget(s) {
    this->pnum=pnum;
  }
  DECL_PTR(c::Touch, tap)
  DECL_IZ(pnum)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Puck : public Widget {
  Puck(not_null<c::Node*> s)
    : Widget(s) {
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Player : public ecs::Component {
  MDECL_COMP_TPID("n/Player")
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
  DECL_FZ(goalWidth)
  DECL_FZ(sq_radii)
  DECL_TD(c::Vec2,ballNextPos)
  DECL_TD(c::Vec2,ballVec)
};



NS_END




