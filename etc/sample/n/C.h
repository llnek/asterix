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
NS_BEGIN(@@APPID@@)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL SpaceShip : public f::ComObj {
  MDECL_COMP_TPID("n/SpaceShip")
  SpaceShip(not_null<c::Sprite*> s)
  : ComObj(s) {
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Asteroid : public f::ComObj {
  MDECL_COMP_TPID("n/Asteroid")
  Asteroid(not_null<c::Sprite*> s)
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

  s_arr<c::Sprite*,2> bgSprites;
};



NS_END




