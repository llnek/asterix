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

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(bazuka)

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


//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Hero : public f::ComObj {
  MDECL_COMP_TPID("n/Hero")
  Hero(not_null<c::Sprite*> s)
  : ComObj(s) {
  }
  virtual ~Hero() {
    CC_DROP(boost);
    CC_DROP(idle);
  }
  DECL_PTR(c::Action,boost);
  DECL_PTR(c::Action,idle);
  DECL_IZ(jumpTimer)
  DECL_BF(jump)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Projectile : public f::ComObj {
  MDECL_COMP_TPID("n/Projectile")
  Projectile(not_null<c::Sprite*> s)
  : ComObj(s) {
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Gesture : public a::Component {
  MDECL_COMP_TPID("n/Gesture")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public a::Component {
  MDECL_COMP_TPID( "n/GVars" )

  DECL_TD(c::Vec2, gravity)

};



NS_END




