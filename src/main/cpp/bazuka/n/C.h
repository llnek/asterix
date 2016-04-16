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

#include "ScrollingBgLayer.h"
#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(bazuka)

//////////////////////////////////////////////////////////////////////////////
//
enum ActionState {
  kActionStateNone = 0,
  kActionStateIdle ,
  kActionStateBoost
};

enum PlayerState {
  kPlayerStateNone = 0,
  kPLayerStateIdle,
  kPlayerStateBoost
};

enum ProjectileType {
  kTypeRocket=0,
  kTypeBullet
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Hero : public f::CPixie {
  Hero() {}
public:

  static owner<Hero*> create() {
    auto z= mc_new(Hero);
    z->initWithSpriteFrameName("player.png");
    z->autorelease();
    return z;
  }

  virtual ~Hero() {
    CC_DROP(boost);
    CC_DROP(idle);
  }
  __decl_ptr(c::Action,boost);
  __decl_ptr(c::Action,idle);
  __decl_iz(jumpTimer)
  __decl_iz(action)
  __decl_iz(state)
  __decl_bf(jump)

  void animate() {
    if (state==kPlayerStateBoost) { animXXX(kActionStateBoost,boost);  }
    if (state== kPLayerStateIdle) { animXXX(kActionStateIdle,idle);  }
  }

  void animXXX(int a, c::Action *act) {
    if (action != a) {
      action = a;
      this->stopAllActions();
      this->runAction(act->clone());
    }
  }

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Projectile : public f::CPixie {
  Projectile(int t) {
    type = t;
  }
public:

  __decl_iz(type)

  static owner<Projectile*> create( int type) {
    auto z= mc_new1(Projectile, type);
    auto png="";
    if (type == kTypeRocket) {
       png= "rocket.png" ;
    }
    else if (type == kTypeBullet) {
      png="bullet.png";
    }
    else {
       throw "bad projectile type";
    }
    z->initWithSpriteFrameName(png);
    z->autorelease();
    return z;
  }

  void sync() {
    if (type == kTypeBullet) {
      this->setPositionX(this->getPositionX() - 7);
    }
    if (type == kTypeRocket) {
      this->setPositionX(this->getPositionX() + 7);
    }
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {
  __decl_comp_tpid( "n/GVars" )

  __decl_ptr(ScrollingBgLayer, bgLayer)
  __decl_md(CCT_PT, gravity)

};



NS_END




