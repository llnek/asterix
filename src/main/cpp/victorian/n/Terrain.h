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

#include "Player.h"
#include "Block.h"
#include "C.h"

#define TILE_H_SIZE 6
#define TILE_W_SIZE 8

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(victorian)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Terrain : public Widget {

  Vector<Block*> _blockPool;
  Vector<Block*> _blocks;

  DECL_IZ(_blockPoolIndex)
  DECL_IZ(_lastBlockHeight)
  DECL_IZ(_lastBlockWidth)
  DECL_IZ(_minTerrainWidth)

  DECL_BF(_showGap)

  DECL_IZ(_currentPatternIndex)
  DECL_IZ(_currentPatternCnt)
  DECL_IZ(_currentWidthIndex)
  DECL_IZ(_currentHeightIndex)
  DECL_IZ(_currentTypeIndex)

  DECL_IZ(_increaseGapInterval)
  DECL_FZ(_increaseGapTimer)
  DECL_IZ(_gapSize)

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

  Terrain(not_null<c::Sprite*> s);
  virtual ~Terrain();
  static Terrain* create();

  void activateChimneysAt(Player*);
  void checkCollision(Player*);

  void move(float xMove);
  void reset();

  CC_SYNTHESIZE(bool, _startTerrain, StartTerrain);
};


NS_END


