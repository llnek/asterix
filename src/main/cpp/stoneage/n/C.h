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
#include "Gem.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(stoneage)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Particle : public f::ComObj {
  MDECL_COMP_TPID("n/Particle")
  Particle(not_null<c::Node*> n)
  : ComObj(n) {
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Diamond : public f::ComObj {
  MDECL_COMP_TPID("n/Diamond")
  Diamond(not_null<c::Node*> n)
  : ComObj(n) {
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GemInfo {
  GemInfo(int x, int y, Gem *g) {
    this->x =x;
    this->y = y;
    this->gem=g;
  }
  DECL_IZ(y)
  DECL_IZ(x)
  DECL_PTR(Gem,gem)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Player : public f::ComObj {
  MDECL_COMP_TPID("n/Player")
  Player() {
  }
};


//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Gesture : public a::Component {
  MDECL_COMP_TPID("n/Gesture")
};




NS_END




