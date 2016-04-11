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

int patterns[] = {1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,3,3,3};
int widths[] = {2,2,2,2,2,3,3,3,3,3,3,4,4,4,4,4,4};
int heights[] = {0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,2,2,2,3,3,3,3,3,3,4};
int types[] = {1,2,3,4,1,3,2,4,3,2,1,4,2,3,1,4,2,3,1,2,3,2,3,4,1,2,4,3,1,3,1,4,2,4,2,1,2,3};

s_vec<int> _blockPattern(patterns, patterns + SIZEOFX(patterns,int));
s_vec<int> _blockWidths(widths, widths + SIZEOFX(widths,int));
s_vec<int> _blockHeights(heights, heights + SIZEOFX(heights,int));
s_vec<int> _blockTypes(types, types + SIZEOFX(types,int));

//////////////////////////////////////////////////////////////////////////////
//
Terrain::~Terrain () {
  _blockPool.clear();
  _blocks.clear();
}

//////////////////////////////////////////////////////////////////////////////
//
owner<Terrain*> Terrain::create() {
  auto t= mc_new(Terrain);
  t->initWithFile("pics/blank.png");
  t->autorelease();
  return t;
}

//////////////////////////////////////////////////////////////////////////////
//
bool Terrain::initWithFile(const sstr &fn) {

  auto rc= c::Sprite::initWithFile(fn);
  auto wz= cx::visSize();

  if (!rc) { return false; }

  S__MIX(_blockHeights);
  S__MIX(_blockPattern);
  S__MIX(_blockWidths);

  _minTerrainWidth = wz.width * 1.5;
  setAnchorPoint(cx::anchorBL());
  _increaseGapInterval = 5000;
  _increaseGapTimer = 0;
  _gapSize = 2;
  _visSize=wz;

    //init object pools
  for (auto i = 0; i < 20; ++i) {
    auto b= Block::create();
    this->addChild(b);
    _blockPool.pushBack(b);
  }

  addBlocks(0);

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Terrain::checkCollision(ecs::Node *node) {

  auto pm=CC_GEC(PlayerMotion,node,"f/CMove");
  auto py=CC_GEC(Player,node,"f/CPixie");
  auto ps=CC_GEC(PlayerStats,node,"f/CStats");
  bool inAir = true;

  if (ps->state == kPlayerDying) {
  return; }

  auto ph= cx::getHeight(py);
  auto pw= cx::getWidth(py);

  for (auto b: _blocks) {
    if (b->getType() == kBlockGap) { continue; }
      /*
      auto w=cx::getWidth(b);
      auto h=cx::getHeight(b);
      auto l=cx::getLeft(b);
      auto r=cx::getRight(b);
      auto t=cx::getTop(b);
      auto bt=cx::getBottom(b);
      */
    //if within x, check y (bottom collision)
    if (cx::getRight(py) >= this->getPositionX() + cx::getLeft(b) &&
        cx::getLeft(py) <= this->getPositionX() + cx::getRight(b)) {

      if (cx::getBottom(py) >= cx::getTop(b) &&
          pm->nextPos.y-ph <= cx::getTop(b) &&
          cx::getTop(py) > cx::getTop(b)) {

        pm->nextPos= CCT_PT(pm->nextPos.x, cx::getTop(b) + ph);
        pm->vel.y=0;
        py->setRotation(0);
        inAir = false;
        break;
      }
    }
  }

  for (auto b: _blocks) {
    if (b->getType() == kBlockGap) { continue; }
    //now if within y, check x (side collision)
    if ((cx::getBottom(py) < cx::getTop(b) &&
          cx::getTop(py) > cx::getBottom(b)) ||
        (pm->nextPos.y-ph < cx::getTop(b) &&
         pm->nextPos.y > cx::getBottom(b))) {

      if (cx::getRight(py) >= this->getPositionX() + b->getPositionX() &&
          cx::getLeft(py) < this->getPositionX() + b->getPositionX()) {

        py->setPositionX(this->getPositionX() + b->getPositionX() - HTV(pw));
        pm->nextPos= CCT_PT(this->getPositionX() + b->getPositionX() - HTV(pw),
                             pm->nextPos.y);
        pm->vel.x= pm->vel.x * -0.5;
        if (cx::getBottom(py) + ph * 0.2 < cx::getTop(b)) {
          ps->state= kPlayerDying;
          return;
        }
        break;
      }
    }
  }

  if (inAir) {
    ps->state= kPlayerFalling;
  } else {
    ps->state= kPlayerMoving;
    pm->setFloating(false);
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void Terrain::move(float xMove) {

  if (xMove < 0) { return; }
  if (_startTerrain) {
    if (xMove > 0 && _gapSize < 5) { _increaseGapTimer += xMove; }
    if (_increaseGapTimer > _increaseGapInterval) {
      _increaseGapTimer = 0;
      _gapSize += 1;
    }
  }

  this->setPositionX(this->getPositionX() - xMove);

  auto pos= this->getPosition();
  auto block = _blocks.at(0);
  auto bw= cx::getWidth(block);
  if (pos.x + bw < 0) {
    auto first= _blocks.at(0);
    _blocks.erase(0);
    _blocks.pushBack(first);
    this->setPositionX(pos.x + bw);
    auto width_cnt = totalWidth() - bw - cx::getWidth(_blocks.at(0));
    this->initBlock(block);
    this->addBlocks(width_cnt);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Terrain::reset() {

  this->setPosition(CC_ZPT);
  _startTerrain = false;

  float currentWidth = 0;
  for (auto b: _blocks) {
    this->initBlock(b);
    currentWidth += cx::getWidth(b);
  }

  addBlocks(currentWidth);

  _increaseGapTimer = 0;
  _gapSize = 2;
}

//////////////////////////////////////////////////////////////////////////////
//
void Terrain::addBlocks(float currentWidth) {

  while (currentWidth < _minTerrainWidth) {
    auto b= _blockPool.at(_blockPoolIndex);

    if (++_blockPoolIndex == _blockPool.size()) {
    _blockPoolIndex = 0; }

    this->initBlock(b);
    _blocks.pushBack(b);

    currentWidth += cx::getWidth(b);
  }

  this->distributeBlocks();
}

//////////////////////////////////////////////////////////////////////////////
//
void Terrain::distributeBlocks() {
  for (auto i= 0; i < _blocks.size(); ++i) {
    auto b= _blocks.at(i);
    if (i != 0) {
      auto prev= _blocks.at(i - 1);
      b->setPositionX(prev->getPositionX() + cx::getWidth(prev));
    } else {
      b->setPositionX(0);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Terrain::initBlock(Block *block) {

  int type = _blockTypes[_currentTypeIndex];

  if (++_currentTypeIndex == _blockTypes.size()) {
  _currentTypeIndex = 0; }

  //terrain is not being changed yet
  if (!_startTerrain) {
    _lastBlockWidth = rand() % 2 + 2;
    _lastBlockHeight = 2;
    block->setupBlock(_lastBlockWidth, _lastBlockHeight, type);
    return;
  }

  //check if min distance reached;

  if (_showGap) {
    auto gap = rand() % _gapSize;
    if (gap < 2) { gap = 2; }
    block->setupBlock(gap,0, kBlockGap);
    _showGap = false;
    return;
  }

  auto blockWidth= _blockWidths[_currentWidthIndex];
  auto blockHeight= _lastBlockHeight;

  if (++_currentWidthIndex == _blockWidths.size()) {
    S__MIX(_blockWidths);
    _currentWidthIndex = 0;
  }

  if (_blockHeights[_currentHeightIndex] != 0) {

    //change height of next block
    blockHeight = _blockHeights[_currentHeightIndex];
    //if difference too high, decrease it
    if (blockHeight - _lastBlockHeight > 2 && _gapSize == 2) {
      blockHeight = 1;
    }

  }

  if (++_currentHeightIndex == _blockHeights.size()) {
    _currentHeightIndex = 0;
    S__MIX(_blockHeights);
  }

  block->setupBlock(blockWidth,blockHeight, type);
  _lastBlockHeight = blockHeight;
  _lastBlockWidth = blockWidth;

  //select next block series pattern
  if (++_currentPatternCnt > _blockPattern[_currentPatternIndex]) {
    _showGap = true;
    //start new pattern
    if (++_currentPatternIndex == _blockPattern.size()) {
      S__MIX(_blockPattern);
      _currentPatternIndex = 0;
    }
    _currentPatternCnt = 1;
  }

}

NS_END



