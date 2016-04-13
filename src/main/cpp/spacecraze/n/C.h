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

#include "x2d/GameScene.h"
#include "core/COMP.h"
#include "core/CCSX.h"
NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(spacecraze)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Alien : public f::CPixie {

  Alien(int n) {
    assert(n > 0 && n < 4);
    _type=n;
  }

  __decl_iz(_type)

public:

  static owner<Alien*> create(int);
  __decl_getr(int,_type,Type)

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Missile : public f::CPixie {

  static owner<Missile*> create(const sstr&);

  void morph(int type) {
    assert(type > 0 && type < 4);
    this->setSpriteFrame("sfbullet" + FTOS(type));
  }

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Ship : public f::CPixie {

  static owner<Ship*> create();

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL AlienSquad : public ecs::Component {
  __decl_comp_tpid("n/AlienSquad")
  __decl_fz(_rightEdge)
  __decl_fz(_leftEdge)
  __decl_fz(_duration)

public:

  __decl_gsms(float,_rightEdge,RightEdge)
  __decl_gsms(float,_leftEdge,LeftEdge)
  __decl_gsms(float,_duration,Duration)

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {
  __decl_comp_tpid("n/GVars")

};

//////////////////////////////////////////////////////////////////////////////
//
void spawnPlayer(not_null<ecs::Node*>);
j::json loadLevel(int n);




NS_END


