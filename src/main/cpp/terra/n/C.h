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
#include "lib.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Missile : public f::CPixie {

  __decl_mv(Attacks, attackMode, Attacks::NORMAL)
  __decl_comp_tpid("f/CPixie")

  Missile(not_null<c::Sprite*> s, Attacks m)
  : CPixie(s) {
    attackMode=m;
  }
  Missile(not_null<c::Sprite*> s)
  : CPixie(s) {
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Bomb : public f::CPixie {

  __decl_mv(Attacks, attackMode, Attacks::NORMAL)
  __decl_comp_tpid("f/CPixie")

  Bomb(not_null<c::Node*> s, Attacks m)
  : CPixie(s) {
    attackMode=m;
  }
  Bomb(not_null<c::Node*> s)
    : CPixie(s) {
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Enemy : public f::CStats {

  __decl_md(EnemyType, enemyType)
  __decl_fz(delayTime)
  __decl_comp_tpid( "n/Enemy")

  Enemy(const EnemyType &et)
  : CStats(et.scoreValue) {
    delayTime= 1.2 * c::rand_0_1() + 1.0;
    enemyType= et;
  }

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Ship : public f::CPixie {

  __decl_ptr(c::Sprite, bornSprite)
  __decl_bf(canBeAttack)
  __decl_comp_tpid( "f/CPixie")

  Ship(not_null<c::Sprite*> s,
       not_null<c::Sprite*> x) 
    : CPixie(s) {
    bornSprite = x;
  }

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Spark : public f::CPixie {

  Spark(not_null<c::Sprite*> sp1, not_null<c::Sprite*> sp2)
    : CPixie(sp1) {
    sprite2= sp2;
  }

  virtual void inflate(float, float);
  virtual void deflate();

  __decl_mv(float, duration, 0.7f)
  __decl_mv(float, scale, 1.2f)
  __decl_ptr(c::Sprite, sprite2)
  __decl_comp_tpid( "f/CPixie")

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Explosion : public f::CPixie {

  virtual void inflate(float x, float y);

  Explosion(not_null<c::Sprite*> s)
    : CPixie(s) {
  }

  __decl_comp_tpid( "f/CPixie")

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL HitEffect : public f::CPixie {

  virtual void inflate(float x, float y);

  HitEffect(not_null<c::Sprite*> s)
    : CPixie(s) {
  }

  __decl_mv(float, scale, 0.75)
  __decl_comp_tpid("f/CPixie")

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {
  __decl_comp_tpid("n/GVars")
  __decl_iz(secCount)
  __decl_ptr(ecs::Node, backSkyRe)
  __decl_ptr(ecs::Node, backSky)
  __decl_md(c::Size, backSkyDim)
};



NS_END


