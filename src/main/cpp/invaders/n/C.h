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

#include "2d/CCActionInterval.h"
#include "core/COMP.h"
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL AlienSquad : public ecs::Component {

  AlienSquad(not_null<f::FPool*> aliens, int step) {
    this->aliens=aliens;
    this->stepx=step;
  }

  __decl_comp_tpid("n/AlienSquad")

  //not owner of pool
  __decl_ptr(f::FPool, aliens)
  __decl_iz(stepx)
};

//////////////////////////////////////////////////////////////////////////////
struct CC_DLL Rank : public f::CStats {
  __decl_comp_tpid( "n/Rank")
  __decl_iz(rank)
  Rank(int v, int r)
  : CStats(v) {
    rank=r;
  }
};

//////////////////////////////////////////////////////////////////////////////
struct CC_DLL ShipStats : public f::CStats {
  __decl_bt(hasAmmo)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Explosion : public f::CPixie {

  static owner<Explosion*> create() {
    auto z= mc_new(Explosion);
    z->initWithSpriteFrameName("boom_0.png");
    z->autorelease();
    return z;
  }

  virtual void inflate(float x, float y) {
    auto anim= CC_ACAC()->getAnimation("boom!");
    f::CPixie::inflate(x,y);
    this->runAction(
      c::Sequence::create(
        c::Animate::create(anim),
        c::CallFunc::create([=]() { this->deflate(); }),
        CC_NIL));
  }

  __decl_comp_tpid("n/Explosion")
  __decl_mv(float,frameTime,0.1)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Ship : public f::CPixie {

  static owner<Ship*> create() {
    auto z= mc_new(Ship);
    z->initWithSpriteFrameName("ship_1.png");
    z->frame1= "ship_1.png";
    z->frame0= "ship_0.png";
    z->autorelease();
    return z;
  }

  __decl_md(sstr, frame0)
  __decl_md(sstr, frame1)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars  : public ecs::Component {

  __decl_md(CCT_SZ, alienSize)
  __decl_md(CCT_SZ, shipSize)
  __decl_md(CCT_SZ, purple)
  __decl_md(CCT_SZ, green)
  __decl_md(CCT_SZ, blue)

  __decl_comp_tpid("n/GVars")

};



NS_END


