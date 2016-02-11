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
#include "core/ComObj.h"
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
Terrain::Terrain(not_null<c::Sprite*> s)
  : Widget(s) {

  _startTerrain=false;
  _blockPoolIndex=0;
  _currentPatternCnt=1;
  _currentPatternIndex=0;
  _currentTypeIndex=0;
  _currentWidthIndex=0;
  _currentHeightIndex=0;
  _blockPool=20;
  _blocks=20;
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
    sprite->addChild(block);
    _blockPool.pushBack(block);
  }

  _minTerrainWidth = wz.size.width * 1.5f;

  random_shuffle(_blockPattern.begin(), _blockPattern.end());
  random_shuffle(_blockWidths.begin(), _blockWidths.end());
  random_shuffle(_blockHeights.begin(), _blockHeights.end());

  this->addBlocks(0);
}

//////////////////////////////////////////////////////////////////////////////
//
void Terrain::activateChimneysAt(Player *player) {

  auto spos= sprite->getPosition();
  auto wb=cx::visBox();

  for (auto i = 0; i < _blocks.size(); ++i) {

    auto block = _blocks.at(i);
    auto pos=block->pos();

    if (block->getType() == kBlockGap) continue;
    if (block->getPuffing()) continue;

    if (spos.x + pos.x >= wb.right * 0.2f &&
        spos.x + pos.x < wb.right * 0.8f) {
      block->setPuffing(true);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Terrain::checkCollision(Player *player) {

  if (player->getState() == kPlayerDying) {
    return;
  }

  auto spos= sprite->getPosition();
  bool inAir = true;

  for (auto block : _blocks) {

    if (block->getType() == kBlockGap) continue;

    //if within x, check y (bottom collision)
    if (player->right() >= spos.x + block->left() &&
        player->left() <= spos.x + block->right()) {

      if (player->bottom() >= block->top() &&
          player->next_bottom() <= block->top() &&
          player->top() > block->top()) {
        player->setNextPosition(
            c::Vec2(player->getNextPosition().x,
            block->top() + player->getHeight()));
        player->vel= c::Vec2(player->vel.x, 0);
        player->setRotation(0.0);
        inAir = false;
        break;
      }
    }
  }

  spos= sprite->getPosition();

  for (auto block : _blocks) {

    if (block->getType() == kBlockGap) continue;

    //now if within y, check x (side collision)
    if ((player->bottom() < block->top() &&
         player->top() > block->bottom()) ||
        (player->next_bottom() < block->top() &&
         player->next_top() > block->bottom())) {

      if (player->right() >= spos.x + block->getPositionX() &&
          player->left() < spos.x + block->getPositionX()) {

        player->sprite->setPositionX(
            spos.x + block->getPositionX() - player->getWidth() * 0.5f);

        player->setNextPosition(
            c::Vec2(spos.x + block->getPositionX() - player->getWidth()*0.5f,
                    player->getNextPosition().y));
        player->vel= c::Vec2(player->vel.x * -0.5f, player->vel.y);
        if (player->bottom() + player->getHeight() * 0.2f < block->top()) {
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
      cx::sfxPlay->pauseEffects();
    }
    player->setFloating (false);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Terrain::move(float xMove) {

  if (xMove < 0) return;

  if (_startTerrain) {
    if (xMove > 0 && _gapSize < 5) _increaseGapTimer += xMove;
    if (_increaseGapTimer > _increaseGapInterval) {
      _increaseGapTimer = 0;
      _gapSize += 1;
    }
  }

  sprite->setPositionX(sprite->getPositionX() - xMove);

  auto pos= sprite->getPosition();
  auto block = _blocks.at(0);
  if (pos.x + block->getWidth() < 0) {
    auto firstBlock = _blocks.at(0);
    _blocks.erase(0);
    _blocks.pushBack(firstBlock);
    sprite->setPositionX(pos.x + block->getWidth());

    auto width_cnt =
      this->getWidth() - block->getWidth() - ( _blocks.at(0))->getWidth();
    this->initBlock(block);
    this->addBlocks(width_cnt);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Terrain::reset() {

  sprite->setPosition(0,0);
  _startTerrain = false;

  int currentWidth = 0;
  for (auto block : _blocks) {
    this->initBlock(block);
    currentWidth +=  block->getWidth();
  }

  while (currentWidth < _minTerrainWidth) {
    auto block =  _blockPool.at(_blockPoolIndex);
    ++_blockPoolIndex;
    if (_blockPoolIndex == _blockPool.size()) {
      _blockPoolIndex = 0;
    }
    _blocks.pushBack(block);
    this->initBlock(block);
    currentWidth +=  block->getWidth();
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
    currentWidth +=  block->getWidth();
    _blocks.pushBack(block);
  }
  this->distributeBlocks();
}

//////////////////////////////////////////////////////////////////////////////
//
void Terrain::distributeBlocks() {

  int count = (int) _blocks.size();
  Block *block;
  Block *prev_block;

  for (auto i = 0; i < count; i++) {
    block =  _blocks.at(i);
    if (i != 0) {
      prev_block = _blocks.at(i - 1);
      block->sprite->setPositionX(
          prev_block->sprite->getPositionX() + prev_block->getWidth());
    } else {
      block->sprite->setPositionX ( 0 );
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


