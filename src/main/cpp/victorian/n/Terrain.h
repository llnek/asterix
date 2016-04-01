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

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(victorian)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Terrain : public f::CPixie {

  virtual bool initWithSpriteFrameName(const sstr&);
  Terrain();

  c::Vector<Block*> _blockPool;
  c::Vector<Block*> _blocks;

  __decl_iz(_minTerrainWidth)
  __decl_iz(_lastBlockHeight)
  __decl_iz(_lastBlockWidth)

  __decl_bf(_showGap)

  __decl_iz(_currentPatternIndex)
  __decl_iz(_currentPatternCnt)
  __decl_iz(_currentWidthIndex)
  __decl_iz(_currentHeightIndex)
  __decl_iz(_currentTypeIndex)
  __decl_iz(_blockPoolIndex)

  __decl_iz(_increaseGapInterval)
  __decl_fz(_increaseGapTimer)
  __decl_iz(_gapSize)
  __decl_bf(_startTerrain)

public:

  void addBlocks(int currentWidth);
  void distributeBlocks();
  void initBlock(Block*);

  virtual float width() {
    int w = 0;
    for (auto b : _blocks) {
      w += cx::getWidth(b);
    }
    return w;
  }

  __decl_gsms_is(_startTerrain, StartTerrain);
  static owner<Terrain*> create();
  virtual ~Terrain();

  void checkCollision(not_null<ecs::Node*>);
  void activateChimneys();
  void reset();
  void move(float xMove);

};


NS_END


