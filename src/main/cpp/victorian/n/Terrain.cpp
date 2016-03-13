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
NS_BEGIN(victorian)

//////////////////////////////////////////////////////////////////////////////
//
static int patterns[] = {1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,3,3,3};
static int widths[] = {2,2,2,2,2,3,3,3,3,3,3,4,4,4,4,4,4};
static int heights[] = {0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,2,2,2,3,3,3,3,3,3,4};
static int types[] = {1,2,3,4,1,3,2,4,3,2,1,4,2,3,1,4,2,3,1,2,3,2,3,4,1,2,4,3,1,3,1,4,2,4,2,1,2,3};

static s_vec<int> _blockPattern(patterns, patterns + sizeof(patterns) / sizeof(int));
static s_vec<int> _blockWidths(widths, widths + sizeof(widths) / sizeof(int));
static s_vec<int> _blockHeights(heights, heights + sizeof(heights) / sizeof(int));
static s_vec<int> _blockTypes(types, types + sizeof(types) / sizeof(int));

//////////////////////////////////////////////////////////////////////////////
//
Terrain::~Terrain () {
  _blockPool.clear();
  _blocks.clear();
}

//////////////////////////////////////////////////////////////////////////////
//
Terrain::Terrain(not_null<c::Node*> s)
  : Widget(s) {

  _startTerrain=false;
  _blockPoolIndex=0;
  _currentPatternCnt=1;
  _currentPatternIndex=0;
  _currentTypeIndex=0;
  _currentWidthIndex=0;
  _currentHeightIndex=0;
  //_blockPool=20;
  //_blocks=20;
  _showGap=false;
}

//////////////////////////////////////////////////////////////////////////////
//
Terrain* Terrain::create() {
  auto s=cx::reifySprite("blank.png");
  auto terrain = mc_new1(Terrain,s);
  terrain->initTerrain();
  return terrain;
}

//////////////////////////////////////////////////////////////////////////////
//
void Terrain::initTerrain() {

  auto wz= cx::visRect();

  _increaseGapInterval = 5000;
  _increaseGapTimer = 0;
  _gapSize = 2;

  //init object pools
  for (auto i = 0; i < 20; ++i) {
    auto block = Block::create();
    node->addChild(block->node);
    _blockPool.pushBack(block);
  }

  _minTerrainWidth = wz.size.width * 1.5;

  random_shuffle(_blockPattern.begin(), _blockPattern.end());
  random_shuffle(_blockWidths.begin(), _blockWidths.end());
  random_shuffle(_blockHeights.begin(), _blockHeights.end());

  this->addBlocks(0);
}

//////////////////////////////////////////////////////////////////////////////
//
void Terrain::activateChimneysAt(Player *player) {

  auto spos= node->getPosition();
  auto wb=cx::visBox();

  for (auto i = 0; i < _blocks.size(); ++i) {

    auto block = _blocks.at(i);
    auto pos=block->pos();

    if (block->getType() == kBlockGap ||
        block->getPuffing())  {
    continue; }

    if (spos.x + pos.x >= wb.right * 0.2 &&
        spos.x + pos.x < wb.right * 0.8) {
      block->setPuffing(true);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Terrain::checkCollision(Player *player) {

  if (player->getState() == kPlayerDying) {
  return; }

  auto spos= node->getPosition();
  bool inAir = true;

  for (auto block : _blocks) {

    if (block->getType() == kBlockGap) {
    continue; }

    //if within x, check y (bottom collision)
    if (player->right() >= spos.x + block->left() &&
        player->left() <= spos.x + block->right()) {

      if (player->bottom() >= block->top() &&
          player->next_bottom() <= block->top() &&
          player->top() > block->top()) {
        player->nextPos=
            c::Vec2(player->nextPos.x, block->top() + player->height());
        player->vel= c::Vec2(player->vel.x, 0);
        player->node->setRotation(0);
        inAir = false;
        break;
      }
    }
  }

  spos= node->getPosition();

  for (auto block : _blocks) {

    if (block->getType() == kBlockGap) {
    continue; }

    //now if within y, check x (side collision)
    if ((player->bottom() < block->top() &&
         player->top() > block->bottom()) ||
        (player->next_bottom() < block->top() &&
         player->next_top() > block->bottom())) {

      if (player->right() >= spos.x + block->node->getPositionX() &&
          player->left() < spos.x + block->node->getPositionX()) {

        player->node->setPositionX(
            spos.x + block->node->getPositionX() - player->width() * 0.5);

        player->nextPos=
            c::Vec2(spos.x + block->node->getPositionX() - player->width()*0.5, player->nextPos.y);
        player->vel= c::Vec2(player->vel.x * -0.5, player->vel.y);

        if (player->bottom() + player->height() * 0.2 < block->top()) {
          player->setState(kPlayerDying);
          cx::sfxPlay("hitBuilding");
          return;
        }
        break;
      }
    }
  }

  if (inAir) {
    player->setState(kPlayerFalling);
  } else {
    if (player->getState() != kPlayerMoving) {
      player->setState(kPlayerMoving);
      cx::pauseEffects();
    }
    player->setFloating (false);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Terrain::move(float xMove) {

  if (xMove < 0)
  return;

  if (_startTerrain) {
    if (xMove > 0 && _gapSize < 5) {
      _increaseGapTimer += xMove;
    }
    if (_increaseGapTimer > _increaseGapInterval) {
      _increaseGapTimer = 0;
      _gapSize += 1;
    }
  }

  node->setPositionX(node->getPositionX() - xMove);

  auto spos= node->getPosition();
  auto block = _blocks.at(0);
  if (spos.x + block->width() < 0) {
    auto firstBlock = _blocks.at(0);
    _blocks.erase(0);
    _blocks.pushBack(firstBlock);
    node->setPositionX(spos.x + block->width());

    auto width_cnt = width() - block->width() - _blocks.at(0)->width();
    this->initBlock(block);
    this->addBlocks(width_cnt);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Terrain::reset() {

  node->setPosition(0,0);
  _startTerrain = false;

  int currentWidth = 0;
  for (auto block : _blocks) {
    this->initBlock(block);
    currentWidth +=  block->width();
  }

  while (currentWidth < _minTerrainWidth) {
    auto block =  _blockPool.at(_blockPoolIndex);
    ++_blockPoolIndex;
    if (_blockPoolIndex == _blockPool.size()) {
      _blockPoolIndex = 0;
    }
    _blocks.pushBack(block);
    this->initBlock(block);
    currentWidth +=  block->width();
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
    currentWidth += block->width();
    _blocks.pushBack(block);
  }
  this->distributeBlocks();
}

//////////////////////////////////////////////////////////////////////////////
//
void Terrain::distributeBlocks() {

  int count = (int) _blocks.size();
  for (auto i = 0; i < count; ++i) {
    auto block =  _blocks.at(i);
    if (i != 0) {
      auto prev_block = _blocks.at(i - 1);
      block->node->setPositionX(
          prev_block->node->getPositionX() + prev_block->width());
    } else {
      block->node->setPositionX(0);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Terrain::initBlock(Block *block) {

  int blockWidth;
  int blockHeight;

  block->setPuffing(false);

  int type = _blockTypes[_currentTypeIndex];
  ++_currentTypeIndex;

  if (_currentTypeIndex == _blockTypes.size()) {
    _currentTypeIndex = 0;
  }

  //check if min distance reached;
  if (_startTerrain) {

    if (_showGap) {

      int gap = rand() % _gapSize;
      if (gap < 2) gap = 2;

      block->setupBlock(gap, 0, kBlockGap);
      _showGap = false;

    } else {

      blockWidth = _blockWidths[_currentWidthIndex];
      ++_currentWidthIndex;

      if (_currentWidthIndex == _blockWidths.size()) {
        random_shuffle(_blockWidths.begin(), _blockWidths.end());
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
        random_shuffle(_blockHeights.begin(), _blockHeights.end());
      }

      block->setupBlock (blockWidth, blockHeight, type);
      _lastBlockWidth = blockWidth;
      _lastBlockHeight = blockHeight;

      //select next block series pattern
      ++_currentPatternCnt;
      if (_currentPatternCnt > _blockPattern[_currentPatternIndex]) {
        _showGap = true;
        //start new pattern
        ++_currentPatternIndex;
        if (_currentPatternIndex == _blockPattern.size()) {
          random_shuffle(_blockPattern.begin(), _blockPattern.end());
          _currentPatternIndex = 0;
        }
        _currentPatternCnt = 1;
      }
    }
    //terrain is not being changed yet
  } else {
    _lastBlockHeight = 2;
    _lastBlockWidth = rand() % 2 + 2;
    block->setupBlock (_lastBlockWidth, _lastBlockHeight, type);
  }
}


NS_END


