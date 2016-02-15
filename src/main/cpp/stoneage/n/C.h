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
NS_BEGIN(stoneage)

enum GemTypes {
  TYPE_GEM_BLUE  = 1,
  TYPE_GEM_YELLOW ,
  TYPE_GEM_RED ,
  TYPE_GEM_GREEN ,
  TYPE_GEM_PINK  ,
  TYPE_GEM_WHITE
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

  self.schedulerID = nil
  self.grid = {}
  self.gridController = nil
  self.gridAnimations = nil
  self.objectPools = nil
  self.gridGemsColumnMap = {};
  self.allGems = {};
  self.gemsContainer = cc.Node:create();
  self.selectedGem = nil
  self.targetGem = nil
  self.selectedIndex = {x = 0, y = 0}
  self.targetIndex = {x = 0, y = 0}
  self.selectedGemPosition = {x = 0, y = 0}
  self.combos = 0
  self.addingCombos = false
  self.scoreLabel = nil
  self.diamondScoreLabel = nil
  self.diamondScore = 0
  self.gemsScore = 0
  self.running = true

};



NS_END




