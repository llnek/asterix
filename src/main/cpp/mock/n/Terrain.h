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

#include "core/CCSX.h"
#include "core/COMP.h"
#include "Block.h"
#include "Player.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(mock)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Terrain : public f::CPixie {

  c::Vector<Block*> _blockPool;
  c::Vector<Block*> _blocks;

  DECL_FZ(_lastBlockHeight)
  DECL_FZ(_lastBlockWidth)
  DECL_TD(c::Rect,_visRect)

  DECL_TV(int,_currentPatternCnt,1)
  DECL_IZ(_currentPatternIndex)
  DECL_IZ(_blockPoolIndex)
  DECL_IZ(_currentWidthIndex)
  DECL_IZ(_currentHeightIndex)
  DECL_IZ(_currentTypeIndex)

  DECL_FZ(_minTerrainWidth)
  DECL_IZ(_gapSize)
  DECL_BF(_startTerrain)
  DECL_BF(_showGap)
  DECL_FZ(_increaseGapInterval)
  DECL_FZ(_increaseGapTimer)

  void addBlocks(float currentWidth);
  void distributeBlocks();
  void initBlock(Block*);

  float totalWidth() {
    float width = 0;
    for (auto block : _blocks) {
      width += cx::getWidth(block);
    }
    return width;
  }

  virtual bool initWithFile(const sstr&);
  Terrain() {}

public:

  DECL_GSMS_IS(bool, _startTerrain, StartTerrain);
  static owner<Terrain*> create();

  virtual ~Terrain();

  void checkCollision(ecs::Node* player);
  void move (float xMove);
  void reset();
};


NS_END


