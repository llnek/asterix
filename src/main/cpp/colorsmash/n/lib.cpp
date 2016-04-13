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

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "lib.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(colorsmash)

//////////////////////////////////////////////////////////////////////////////
//
void findTilesToRemove(const s_vec<int> &tiles,
    s_vec<int> &garbo,
    int col, int row, int tileColor) {

  // first do bounds checking
  if (col < 0 || col >= NUM_COLS || row < 0 || row >= NUM_ROWS) {
  return; }

  auto tileId = row * NUM_COLS + col;

  // now check if tile is of required colour
  if (tiles[tileId] != tileColor) {
  return; }

  // check if tile is already saved
  if (s::find(garbo.begin(),garbo.end(),tileId)
      != garbo.end()) {
  return; }

  // save the tile to be removed
  garbo.push_back(tileId);

  // check up
  findTilesToRemove(tiles, garbo, col, row+1, tileColor);

  // check down
  findTilesToRemove(tiles, garbo, col, row-1, tileColor);

  // check left
  findTilesToRemove(tiles, garbo, col-1, row, tileColor);

  // check right
  findTilesToRemove(tiles, garbo, col+1, row, tileColor);
}

//////////////////////////////////////////////////////////////////////////////
//
c::Vec2 getRandomPositionForTile() {
  return c::Vec2(floor(1 + cx::rand() * NUM_COLS) * TILE_SIZE,
      floor(1 + cx::rand() * (NUM_ROWS+5)) * TILE_SIZE );
}

//////////////////////////////////////////////////////////////////////////////
//
c::Vec2 getPositionForTile(int id) {
  // calculate column and row for given tile id
  auto col = floor(id % NUM_COLS);
  auto row = floor(id / NUM_COLS);
  auto x=CC_CSV(c::Float, "GAMEPLAY+OFFSET+X");
  auto y=CC_CSV(c::Float, "GAMEPLAY+OFFSET+Y");
  // return point based on the column and row of the tile
  return c::ccpAdd(
      c::Vec2(x,y),
      c::Vec2(col * TILE_SIZE + TILE_SIZE/2, row * TILE_SIZE + TILE_SIZE/2));
}

//////////////////////////////////////////////////////////////////////////////
//
c::Color3B getColorForTile(int colorData) {
  switch (colorData) {
    case E_COLOR_RED:
      return c::Color3B(255, 128, 0);
    break;
    case E_COLOR_GREEN:
      return c::Color3B(0, 204, 0);
    break;
    case E_COLOR_BLUE:
      return c::Color3B(0, 76, 153);
    break;
    case E_COLOR_YELLOW:
      return c::Color3B(102, 0, 102);
    break;
  }

  throw "bad color!";
}




NS_END




