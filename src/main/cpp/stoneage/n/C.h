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

class GridController;
class GridAnimations;
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
struct CC_DLL Gesture : public a::Component {
  MDECL_COMP_TPID("n/Gesture")
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public a::Component {
  MDECL_COMP_TPID( "n/GVars" )
  s_arr<int,GEMSET_SIZE> TYPES {
    TYPE_GEM_BLUE,
    TYPE_GEM_YELLOW,
    TYPE_GEM_RED,
    TYPE_GEM_GREEN,
    TYPE_GEM_PINK,
    TYPE_GEM_WHITE
  };

  s_vec<f::FArrayPtr<Gem>*> gridGemsColumnMap;
  s_vec<f::FArrInt*> grid;
  s_vec<Gem*> allGems;

  DECL_PTR(GridController, gridController)
  DECL_PTR(GridAnimations, gridAnimations)
  DECL_PTR(c::Node,gemsContainer)
  DECL_BF(enabled)
  DECL_PTR(Gem,selectedGem)
  DECL_PTR(Gem,targetGem)
  DECL_TD(f::Cell2P,selectedIndex)
  DECL_TD(f::Cell2P,targetIndex)
  DECL_TD(c::Vec2,selectedGemPosition)
  DECL_IZ(combos)
  DECL_BF(addingCombos)
};



NS_END




