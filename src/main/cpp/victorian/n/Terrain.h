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

#include "core/ComObj.h"
#include "Block.h"
#include "Player.h"

#define TILE_H_SIZE 6
#define TILE_W_SIZE 8

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(victorian)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Terrain : public Widget {

  Vector<Block*> _blockPool;
  Vector<Block*> _blocks;

  int _blockPoolIndex;
  int _lastBlockHeight;
  int _lastBlockWidth;
  int _minTerrainWidth;

  bool _showGap;

  int _currentPatternIndex;
  int _currentPatternCnt;
  int _currentWidthIndex;
  int _currentHeightIndex;
  int _currentTypeIndex;

  int _increaseGapInterval;
  float _increaseGapTimer;
  int _gapSize;

  void initTerrain();
  void addBlocks(int currentWidth);

  void distributeBlocks();
  void initBlock(Block *block);

  virtual float getWidth () {
    int width = 0;
    for (auto block : _blocks) {
      width += block->getWidth();
    }
    return width;
  }

  Terrain(not_null<c::Sprite*> s);
  virtual ~Terrain();
  static Terrain* create();

  void activateChimneysAt(Player *player);
  void checkCollision(Player *player);

  void move(float xMove);
  void reset();

  CC_SYNTHESIZE(bool, _startTerrain, StartTerrain);
};


NS_END


