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

#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "core/COMP.h"
#include "BBlock.h"
#include "Player.h"

NS_BEGIN(victorian)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Terrain : public f::CPixie {

  c::Vector<Block*> _blockPool;
  c::Vector<Block*> _blocks;

  __decl_fz(_minTerrainWidth)
  __decl_iz(_blockPoolIndex)
  __decl_fz(_lastBlockHeight)
  __decl_fz(_lastBlockWidth)
  __decl_bf(_showGap)
  __decl_bf(_startTerrain)

  __decl_iz(_currentPatternIndex)
  __decl_iz(_currentPatternCnt)
  __decl_iz(_currentWidthIndex)
  __decl_iz(_currentHeightIndex)
  __decl_iz(_currentTypeIndex)
  __decl_iz(_increaseGapInterval)
  __decl_fz(_increaseGapTimer)
  __decl_iz(_gapSize)

  virtual bool initWithSpriteFrameName(const sstr&);
  void addBlocks(float currentWidth);

  void distributeBlocks();
  void initBlock(Block*);
  Terrain();

  float totalWidth() {
    float width = 0;
    for (auto block : _blocks) {
      width += block->getWidth();
    }
    return width;
  }

public:

  __decl_gsms_is(_startTerrain, StartTerrain);
  static owner<Terrain*> create();
  virtual ~Terrain();

  void checkCollision(not_null<ecs::Node*>);
  void activateChimneys();
  void move (float xMove);
  void reset();

};


NS_END


