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
#include "core/ComObj.h"
#include "core/CCSX.h"
#include "lib.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Missile : public f::ComObj {

  DECL_TV(Attacks, attackMode, Attacks::NORMAL)
  MDECL_COMP_TPID( "n/Missile")

  Missile(not_null<c::Sprite*> s, Attacks m = Attacks::NORMAL)
    : ComObj(s,1,0) {
    speed.y= CC_CSV(c::Float, "MISSILE+SPEED");
    speed.x= 0;
    vel.y= speed.y;
    vel.x= 0;
    attackMode = m;
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Bomb : public f::ComObj {

  DECL_TV(Attacks, attackMode, Attacks::NORMAL)
  MDECL_COMP_TPID( "n/Bomb")

  Bomb(not_null<c::Sprite*> s, Attacks m = Attacks::NORMAL)
    : ComObj(s,1,0) {
    speed.y= - CC_CSV(c::Float, "BOMB+SPEED");
    speed.x= 0;
    vel.y= speed.y;
    vel.x= 0;
    attackMode = m;
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Enemy : public f::ComObj {

  DECL_TD(EnemyType, enemyType)
  DECL_FZ(delayTime)
  MDECL_COMP_TPID( "n/Enemy")

  Enemy(not_null<c::Sprite*> s, const EnemyType &et)
    : ComObj(s, et.HP, et.scoreValue) {
    delayTime= 1.2f * c::rand_0_1() + 1.0f;
    enemyType= et;
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Ship : public f::ComObj {

  DECL_PTR(c::Sprite, bornSprite)
  DECL_BF(canBeAttack)
  MDECL_COMP_TPID( "n/Ship")

  Ship(not_null<c::Sprite*> s,
       not_null<c::Sprite*> x, int health=5)
    : ComObj(s, health, 0) {
    bornSprite = x;
  }

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Gesture : public a::Component {

  MDECL_COMP_TPID( "n/Gesture")
  DECL_BF(right)
  DECL_BF(left)
  DECL_BF(down)
  DECL_BF(up)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Spark : public f::ComObj {

  Spark(not_null<c::Sprite*> sp1, not_null<c::Sprite*> sp2)
    : ComObj(sp1,1,0) {
    sprite2= sp2;
  }

  virtual void inflate(float, float);
  virtual void deflate();

  DECL_TV(float, duration, 0.7f)
  DECL_TV(float, scale, 1.2f)
  DECL_PTR(c::Sprite, sprite2)
  MDECL_COMP_TPID( "n/Spark")

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Explosion : public f::ComObj {

  virtual void inflate(float x, float y);

  Explosion(not_null<c::Sprite*> s)
    : ComObj(s) {
  }

  MDECL_COMP_TPID( "n/Explosion")

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL HitEffect : public f::ComObj {

  virtual void inflate(float x, float y);

  HitEffect(not_null<c::Sprite*> s)
    : ComObj(s) {
  }

  MDECL_COMP_TPID("n/HitEffect")
  DECL_TV(float, scale, 0.75f)

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public a::Component {
  MDECL_COMP_TPID("n/GVars")
  DECL_IZ(secCount)
};



NS_END(terra)


