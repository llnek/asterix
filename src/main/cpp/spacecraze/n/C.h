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

#include "x2d/GameScene.h"
#include "core/COMP.h"
#include "core/CCSX.h"
NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(spacecraze)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Alien : public f::CPixie {
  __decl_comp_tpid("f/CPixie")
  __decl_iz(type)
  Alien(int n) {
    assert(n > 0 && n < 4);
    node = cx::reifySprite("sfenmy" + FTOS(n));
    type=n;
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Missile : public f::CPixie {
  Missile(not_null<c::Node*> s)
  : CPixie(s) {}
  __decl_comp_tpid("f/CPixie")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Bomb : public f::CPixie {
  __decl_comp_tpid("f/CPixie")
  Bomb(not_null<c::Node*> s)
  : CPixie(s) {}
  void morph(int type) {
    assert(type > 0 && type < 4);
      SCAST(c::Sprite*,node)->setSpriteFrame("sfbullet" + FTOS(type));
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Ship : public f::CPixie {
  __decl_comp_tpid("f/CPixie")
  Ship(not_null<c::Node*> s)
  : CPixie(s) {
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL AlienSquad : public ecs::Component {

  __decl_comp_tpid("n/AlienSquad")
  __decl_fz(rightEdge)
  __decl_fz(leftEdge)
  __decl_fz(duration)

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {
  __decl_comp_tpid("n/GVars")

};


NS_END


