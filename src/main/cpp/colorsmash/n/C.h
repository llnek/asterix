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
NS_BEGIN(colorsmash)

enum Constants {
  MAX_COLORS = 4,            // maximum number of colours we can use
  TILE_SIZE = 32,             // size in points of each tile (same as tile.png)
  NUM_COLS = 14,              // maximum number of columns
  NUM_ROWS = 20,              // maximum number of rows
  SCORE_PER_TILE = 10         // score when a tile is cleared
};

// define an object that we can use an enumeration for our colour types
enum E_COLOR_TYPE  {
    E_COLOR_NONE=0,
    E_COLOR_RED,
    E_COLOR_GREEN,
    E_COLOR_BLUE,
    E_COLOR_YELLOW
};

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




