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

static s_vec<int> _blockPattern(patterns,patterns + SIZEOFX(patterns,int));
static s_vec<int> _blockWidths(widths,widths + SIZEOFX(widths,int));
static s_vec<int> _blockHeights(heights, heights + SIZEOFX(heights,int));
static s_vec<int> _blockTypes(types, types + SIZEOFX(types,int));

//////////////////////////////////////////////////////////////////////////////
//
Terrain::~Terrain () {
  _blockPool.clear();
  _blocks.clear();
}

//////////////////////////////////////////////////////////////////////////////
//
Terrain::Terrain() {
  _currentPatternCnt=1;
}

//////////////////////////////////////////////////////////////////////////////
//
owner<Terrain*> Terrain::create() {
  auto t= mc_new(Terrain);
  t->initWithSpriteFrameName("blank.png");
  t->autorelease();
  return t;
}

//////////////////////////////////////////////////////////////////////////////
//
bool Terrain::initWithSpriteFrameName(const sstr &fn) {

  auto rc= c::Sprite::initWithSpriteFrameName(fn);
  if (!rc) { return false; }
  auto wz= cx::visSize();

  _minTerrainWidth = wz.width * 1.5;
  _increaseGapInterval = 5000;
  _increaseGapTimer = 0;
  _gapSize = 2;

  //init object pools
  for (auto i = 0; i < 20; ++i) {
    auto b= Block::create();
    this->addChild(b);
    _blockPool.pushBack(b);
  }

  S__MIX(_blockPattern);
  S__MIX(_blockWidths);
  S__MIX(_blockHeights);

  this->addBlocks(0);

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Terrain::activateChimneys() {

  auto spos= this->getPosition();
  auto wb=cx::visBox();

  for (auto i = 0; i < _blocks.size(); ++i) {

    auto b= _blocks.at(i);
    auto pos=b->getPosition();

    if (b->getType() == kBlockGap ||
        b->isPuffing())  {
    continue; }

    if (spos.x + pos.x >= wb.right * 0.2 &&
        spos.x + pos.x < wb.right * 0.8) {
      b->setPuffing(true);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Terrain::checkCollision(not_null<ecs::Node*> node) {

  auto ps= CC_GEC(PlayerStats,node,"f/CStats");
  auto py= CC_GEC(Player,node,"f/CPixie");
  auto pm= CC_GEC(PlayerMotion,node,"f/CMove");

  if (ps->state == kPlayerDying) {
  return; }

  auto spos= this->getPosition();
  bool inAir = true;
  auto ph= cx::getHeight(py);
  auto pw= cx::getWidth(py);
  auto px= cx::bbox4(py);

  for (auto b: _blocks) {

    if (b->getType() == kBlockGap) {
    continue; }

    //if within x, check y (bottom collision)
    auto bx= cx::bbox4(b);
    if (px.right >= spos.x + bx.left &&
        px.left <= spos.x + bx.right) {

      if (px.bottom >= bx.top &&
          pm->nextPos.y - ph <= bx.top &&
          px.top > bx.top) {
        pm->nextPos.y= bx.top + ph;
        pm->vel.y= 0;
        py->setRotation(0);
        inAir = false;
        break;
      }
    }
  }

  spos= this->getPosition();
  px= cx::bbox4(py);
  for (auto b : _blocks) {

    if (b->getType() == kBlockGap) {
    continue; }

    //now if within y, check x (side collision)
    auto bx= cx::bbox4(b);
    if ((px.bottom < bx.top && px.top > bx.bottom) ||
        (pm->nextPos.y - ph < bx.top &&
         pm->nextPos.y > bx.bottom)) {

      if (px.right >= spos.x + b->getPositionX() &&
          px.left < spos.x + b->getPositionX()) {

        py->setPositionX( spos.x + b->getPositionX() - HTV(pw));
        pm->nextPos.x= spos.x + b->getPositionX() - HTV(pw);
        pm->vel.x= pm->vel.x * -0.5;

        px= cx::bbox4(py);
        if (px.bottom + ph * 0.2 < bx.top) {
          ps->state= kPlayerDying;
          cx::sfxPlay("hitBuilding");
          return;
        }
        break;
      }
    }
  }

  if (inAir) {
    ps->state= kPlayerFalling;
  } else {
    if (ps->state != kPlayerMoving) {
      ps->state=kPlayerMoving;
      cx::pauseEffects();
    }
    pm->setFloating(py,false);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Terrain::move(float xMove) {

  if (xMove < 0) { return;  }
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
  auto spos= this->getPosition();
  auto b= _blocks.at(0);
  auto bw= cx::getWidth(b);
  if (spos.x + bw < 0) {
    _blocks.erase(0);
    _blocks.pushBack(b);
    this->setPositionX(spos.x + bw);
    auto tw = width() - bw - cx::getWidth(_blocks.at(0));
    this->initBlock(b);
    this->addBlocks(tw);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Terrain::reset() {

  this->setPosition(0,0);
  _startTerrain = false;

  auto cw = 0;
  for (auto b : _blocks) {
    this->initBlock(b);
    cw +=  cx::getWidth(b);
  }

  while (cw < _minTerrainWidth) {
    auto b= _blockPool.at(_blockPoolIndex);
    ++_blockPoolIndex;
    if (_blockPoolIndex == _blockPool.size()) {
      _blockPoolIndex = 0;
    }
    _blocks.pushBack(b);
    this->initBlock(b);
    cw += cx::getWidth(b);
  }

  this->distributeBlocks();
  _increaseGapTimer = 0;
  _gapSize = 2;
}

//////////////////////////////////////////////////////////////////////////////
//
void Terrain::addBlocks(int cw) {

  while (cw < _minTerrainWidth) {
    auto b= _blockPool.at(_blockPoolIndex);
    ++_blockPoolIndex;
    if (_blockPoolIndex == _blockPool.size()) {
      _blockPoolIndex = 0;
    }
    this->initBlock(b);
    cw += cx::getWidth(b);
    _blocks.pushBack(b);
  }

  this->distributeBlocks();
}

//////////////////////////////////////////////////////////////////////////////
//
void Terrain::distributeBlocks() {

  auto count = (int) _blocks.size();
  for (auto i= 0; i< count; ++i) {
    auto b= _blocks.at(i);
    if (i != 0) {
      auto prev= _blocks.at(i-1);
      b->setPositionX(
          prev->getPositionX() + cx::getWidth(prev));
    } else {
      b->setPositionX(0);
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

  block->setPuffing(false);

  //terrain is not being changed yet
  if (!_startTerrain) {
    _lastBlockWidth = rand() % 2 + 2;
    _lastBlockHeight = 2;
    block->setup(CCT_SZ(_lastBlockWidth, _lastBlockHeight), type);
    return;
  }

  //check if min distance reached;
  if (_showGap) {
    auto gap = rand() % _gapSize;
    if (gap < 2) { gap = 2; }
    block->setup(CCT_SZ(gap, 0), kBlockGap);
    _showGap = false;
    return;
  }

  auto blockWidth = _blockWidths[_currentWidthIndex];
  auto blockHeight=0;

  ++_currentWidthIndex;
  if (_currentWidthIndex == _blockWidths.size()) {
    _currentWidthIndex = 0;
    S__MIX(_blockWidths);
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
      S__MIX(_blockHeights);
  }

  block->setup(CCT_SZ(blockWidth, blockHeight), type);
  _lastBlockHeight = blockHeight;
  _lastBlockWidth = blockWidth;

  //select next block series pattern
  ++_currentPatternCnt;
  if (_currentPatternCnt > _blockPattern[_currentPatternIndex]) {
    ++_currentPatternIndex;
    _showGap = true;
    //start new pattern
    if (_currentPatternIndex == _blockPattern.size()) {
      _currentPatternIndex = 0;
        S__MIX(_blockPattern);
    }
    _currentPatternCnt = 1;
  }

}


NS_END


