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

void findTilesToRemove(const s_vec<int> &tiles,
    s_vec<int>&,
    int col, int row, int tileColor);

c::Color3B getColorForTile(int colorData);

c::Vec2 getRandomPositionForTile();

c::Vec2 getPositionForTile(int);

NS_END




