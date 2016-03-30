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

#include "BBlock.h"
#include "Player.h"
#include "C.h"

#define TILE_H_SIZE 6
#define TILE_W_SIZE 8

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(victorian)

class Player;
//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Terrain : public Widget {

  c::Vector<Block*> _blockPool;
  c::Vector<Block*> _blocks;

  __decl_iz(_blockPoolIndex)
  __decl_iz(_lastBlockHeight)
  __decl_iz(_lastBlockWidth)
  __decl_iz(_minTerrainWidth)

  __decl_bf(_showGap)

  __decl_iz(_currentPatternIndex)
  __decl_iz(_currentPatternCnt)
  __decl_iz(_currentWidthIndex)
  __decl_iz(_currentHeightIndex)
  __decl_iz(_currentTypeIndex)

  __decl_iz(_increaseGapInterval)
  __decl_fz(_increaseGapTimer)
  __decl_iz(_gapSize)

  void initTerrain();
  void addBlocks(int currentWidth);

  void distributeBlocks();
  void initBlock(Block*);

  virtual float width () {
    int w = 0;
    for (auto block : _blocks) {
      w += block->width();
    }
    return w;
  }

  Terrain(not_null<c::Node*> s);
  virtual ~Terrain();
  static Terrain* create();

  void activateChimneysAt(Player*);
  void checkCollision(Player*);

  void move(float xMove);
  void reset();

  MDECL_COMP_TPID("n/Terrain")

  CC_SYNTHESIZE(bool, _startTerrain, StartTerrain);
};


NS_END


