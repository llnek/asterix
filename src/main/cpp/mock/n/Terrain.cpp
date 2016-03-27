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

#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"
#include "Terrain.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(mock)

//////////////////////////////////////////////////////////////////////////////
//
static int patterns[] = {1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,3,3,3};
static int widths[] = {2,2,2,2,2,3,3,3,3,3,3,4,4,4,4,4,4};
static int heights[] = {0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,2,2,2,3,3,3,3,3,3,4};
static int types[] = {1,2,3,4,1,3,2,4,3,2,1,4,2,3,1,4,2,3,1,2,3,2,3,4,1,2,4,3,1,3,1,4,2,4,2,1,2,3};

static s_vec<int> _blockPattern(patterns, patterns+SIZEOFX(patterns,int));
static s_vec<int> _blockWidths(widths, widths+SIZEOFX(widths,int));
static s_vec<int> _blockHeights(heights, heights+SIZEOFX(heights,int));
static s_vec<int> _blockTypes(types, types+SIZEOFX(types,int));

//////////////////////////////////////////////////////////////////////////////
//
Terrain::~Terrain () {
  _blockPool.clear();
  _blocks.clear();
}

//////////////////////////////////////////////////////////////////////////////
//
owner<Terrain*> Terrain::create(const c::Rect &frame) {
  auto t= mc_new(Terrain);
  t->initWithFile("pics/blank.png") {
  t->setAnchorPoint(cx::anchorBL());
  t->initTerrain(frame);
  t->autorelease();
  return t;
}

//////////////////////////////////////////////////////////////////////////////
//
void Terrain::initTerrain(const c::Rect &frame) {

  _increaseGapInterval = 5000;
  _increaseGapTimer = 0;
  _gapSize = 2;
  _visRect= frame;

    //init object pools
  for (auto i= 0; i < 20; ++i) {
    auto b= Block::create();
    this->addChild(b);
    _blockPool.pushBack(b);
  }

  _minTerrainWidth = frame.size.width * 1.5;

  s::random_shuffle(_blockPattern.begin(), _blockPattern.end());
  s::random_shuffle(_blockWidths.begin(), _blockWidths.end());
  s::random_shuffle(_blockHeights.begin(), _blockHeights.end());

  this->addBlocks(0);
}

//////////////////////////////////////////////////////////////////////////////
//
void Terrain::checkCollision(ecs::Node *node) {

  auto ps= CC_GEC(PlayerStats,node,"f/CStats");
  auto py= CC_GEC(Player,node,"f/CPixie");
  auto mv= CC_GEC(PlayerMotion,node,"f/CMove");

  if (ps->state == kPlayerDying) {
  return; }

  bool inAir = true;

  for (auto block : _blocks) {

    if (block->isGap()) {
    continue; }

    //if within x, check y (bottom collision)
    if (cx::getRight(py) >= this->getPositionX() + cx::getLeft(block) &&
        cx::getLeft(py) <= this->getPositionX() + cx::getRight(block)) {

      if (cx::getBottom(py) >= cx::getTop(block) &&
          mv->nextPos.y - cx::getHeight(py) <= cx::getTop(block) &&
          cx::getTop() > cx::getTop()) {

        mv->nextPos = c::Vec2(mv->nextPos.x, cx::getTop(block) + cx::getHeight(py));
        mv->vel= c::Vec2(mv.vel.x, 0);
        py->setRotation(0);
        inAir = false;
        break;
      }
    }
  }

  for (auto block : _blocks) {

    if (block->isGap()) {
    continue; }

    //now if within y, check x (side collision)
    if ((cx::getBottom(py) < cx::getTop(block) &&
          cx::getTop(py) > cx::getBottom(block)) ||
        (mv->nextPos.y - cx::getHeight(py) < cx::getTop(block) &&
         mv->nextPos.y > cx::getBottom(block))) {

      if (cx::getRight(py) >= this->getPositionX() + block->getPositionX() &&
          cx::getLeft(py) < this->getPositionX() + block->getPositionX()) {

        py->setPositionX(this->getPositionX() +
                         block->getPositionX() - HTV(py->width()));

        mv->nextPos= c::Vec2(this->getPositionX() + block->getPositionX() - HTV(cx::getWidth(py)), mv.nextPos.y);

        mv->vel= c::Vec2(mv.vel.x * -0.5, mv.vel.y);
        if (cx::getBottom(py) + cx::getHeight(py) * 0.2 < cx::getTop(block)) {
          ps->state= kPlayerDying;
          return;
        }

        break;
      }
    }
  }

  if (inAir) {
    py->state= kPlayerFalling;
  } else {
    py->state= kPlayerMoving;
    py->setFloating(false);
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void Terrain::move(float xMove) {

  if (xMove < 0) { return; }

  if (_startTerrain) {

    if (xMove > 0 && _gapSize < 5) {
      _increaseGapTimer += xMove;
    }

    if (_increaseGapTimer > _increaseGapInterval) {
      _increaseGapTimer = 0;
      _gapSize += 1;
    }
  }

  this->setPositionX(this->getPositionX() - xMove);

  auto pos= this->getPosition();
  auto block = _blocks.at(0);

  if (pos.x + cx::getWidth(block) < 0) {

    auto firstBlock = _blocks.at(0);
    _blocks.erase(0);
    _blocks.pushBack(firstBlock);
    this->setPositionX(pos.x + cx::getWidth(block));

    float width_cnt = cx::getWidth(this) - cx::getWidth(block) - cx::getWidth(_blocks.at(0));

    this->initBlock(block);
    this->addBlocks(width_cnt);
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void Terrain::reset() {

  this->setPosition(0,0);
  _startTerrain = false;

  auto currentWidth = 0;
  for (auto block : _blocks) {
    this->initBlock(block);
    currentWidth += cx::getWidth(block);
  }

  while (currentWidth < _minTerrainWidth) {
    auto block = _blockPool.at(_blockPoolIndex);
    ++_blockPoolIndex;
    if (_blockPoolIndex == _blockPool.size()) {
      _blockPoolIndex = 0;
    }
    _blocks.pushBack(block);
    this->initBlock(block);
    currentWidth += cx::getWidth(block);
  }

  this->distributeBlocks();
  _increaseGapTimer = 0;
  _gapSize = 2;

}

//////////////////////////////////////////////////////////////////////////////
//
void Terrain::addBlocks(int currentWidth) {

  while (currentWidth < _minTerrainWidth) {
    auto block = _blockPool.at(_blockPoolIndex);
    ++_blockPoolIndex;
    if (_blockPoolIndex == _blockPool.size()) {
      _blockPoolIndex = 0;
    }
    this->initBlock(block);
    currentWidth +=  block->width();
    _blocks.pushBack(block);
  }

  this->distributeBlocks();
}

//////////////////////////////////////////////////////////////////////////////
//
void Terrain::distributeBlocks() {

  auto count = _blocks.size();

  for (auto i = 0; i < count; ++i) {
    auto block = _blocks.at(i);
    if (i != 0) {
      auto prev = _blocks.at(i - 1);
      block->setPositionX(prev->getPositionX() + cx::getWidth(prev));
    } else {
      block->setPositionX(0);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Terrain::initBlock(Block *block) {

  auto type = _blockTypes[_currentTypeIndex];
  ++_currentTypeIndex;

  if (_currentTypeIndex == _blockTypes.size()) {
    _currentTypeIndex = 0;
  }

  float blockHeight;
  float blockWidth;

  //check if min distance reached;
  if (_startTerrain) {

    if (_showGap) {

      auto gap = cx::randInt(_gapSize);
      if (gap < 2) { gap = 2; }

      block->setupBlock(_visRect, gap, 0, kBlockGap);
      _showGap = false;
    } else {

      blockWidth = _blockWidths[_currentWidthIndex];
      ++_currentWidthIndex;

      if (_currentWidthIndex == _blockWidths.size()) {
        s::random_shuffle(_blockWidths.begin(), _blockWidths.end());
        _currentWidthIndex = 0;
      }

      if (_blockHeights[_currentHeightIndex] != 0) {

        //change height of next block
        blockHeight = _blockHeights[_currentHeightIndex];
        //if difference too high, decrease it
        if (blockHeight - _lastBlockHeight > 2 && _gapSize == 2) {
          blockHeight = 1;
        }

      } else {
        blockHeight = _lastBlockHeight;
      }

      ++_currentHeightIndex;
      if (_currentHeightIndex == _blockHeights.size()) {
        _currentHeightIndex = 0;
        s::random_shuffle(_blockHeights.begin(), _blockHeights.end());
      }

      block->setupBlock(_visRect, blockWidth, blockHeight, type);
      _lastBlockWidth = blockWidth;
      _lastBlockHeight = blockHeight;

      //select next block series pattern
      ++_currentPatternCnt;
      if (_currentPatternCnt > _blockPattern[_currentPatternIndex]) {
        _showGap = true;
        //start new pattern
        ++_currentPatternIndex;
        if (_currentPatternIndex == _blockPattern.size()) {
          s::random_shuffle(_blockPattern.begin(), _blockPattern.end());
          _currentPatternIndex = 0;
        }
        _currentPatternCnt = 1;
      }
    }

    //terrain is not being changed yet
  } else {
    _lastBlockHeight = 2;
    _lastBlockWidth = rand() % 2 + 2;
    block->setupBlock(_visRect, _lastBlockWidth, _lastBlockHeight, type);
  }

}


NS_END


