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
  MDECL_COMP_TPID("f/CPixie")
  Alien(int n) {
    assert(n > 0 && n < 4);
    node = cx::reifySprite("sfenmy" + s::to_string(n));
    type=n;
    //MGML()->addAtlasItem("game-pics",node);
  }
  DECL_IZ(type)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Missile : public f::CPixie {
  Missile(not_null<c::Node*> s)
  : CPixie(s) {}
  MDECL_COMP_TPID("f/CPixie")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Bomb : public f::CPixie {
  Bomb(not_null<c::Node*> s)
  : CPixie(s) {}
  MDECL_COMP_TPID("n/Bomb")
  void morph(int type) {
    assert(type > 0 && type < 4);
    node->setSpriteFrame(
        "sfbullet" + s::to_string(type));
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Ship : public f::CPixie {
  MDECL_COMP_TPID("f/CPixie")
  Ship(not_null<c::Node*> s)
  : CPixie(s) {
    //speed.x= 250;
    //vel.x=250;
  }
  //virtual const c::Size csize() { return c::Size(64,64);//84,96); }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL AlienSquad : public ecs::Component {

  AlienSquad(not_null<f::FPool*> aliens) {
    this->aliens=aliens;
  }

  MDECL_COMP_TPID("n/AlienSquad")

  //not owner of pool
  DECL_PTR(f::FPool, aliens)
  DECL_FZ(rightEdge)
  DECL_FZ(leftEdge)
  DECL_FZ(duration)

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {
  MDECL_COMP_TPID("n/GVars")

};


NS_END


