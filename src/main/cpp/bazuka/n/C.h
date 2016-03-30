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
struct CC_DLL Hero : public f::CPixie {
  Hero(not_null<c::Node*> s)
  : CPixie(s) {
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
      node->stopAllActions();
      node->runAction(act->clone());
    }
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Projectile : public f::CPixie {
  __decl_iz(type)
  Projectile(not_null<c::Node*> s, int type)
  : CPixie(s) {
    this->type=type;
  }
  void sync() {
    if (type == kTypeBullet) {
      node->setPositionX(node->getPositionX() - 7);
    }
    if (type == kTypeRocket) {
      node->setPositionX(node->getPositionX() + 7);
    }
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {
  MDECL_COMP_TPID( "n/GVars" )

  __decl_ptr(ScrollingBgLayer, bgLayer)
  __decl_td(c::Vec2, gravity)

};



NS_END




