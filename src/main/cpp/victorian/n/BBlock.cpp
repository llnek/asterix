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
#include "BBlock.h"

#define TOTAL_PUFFS 3

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(victorian)

//////////////////////////////////////////////////////////////////////////////
//
Block::~Block () {
  CC_SAFE_RELEASE(_puffAnimation);
  CC_SAFE_RELEASE(_puffSpawn);
  CC_SAFE_RELEASE(_puffMove);
  CC_SAFE_RELEASE(_puffFade);
  CC_SAFE_RELEASE(_puffScale);
  _chimneys.clear();
  _wallTiles.clear();
  _roofTiles.clear();
}

//////////////////////////////////////////////////////////////////////////////
//
Block::Block (not_null<c::Sprite*> s)
  : Widget(s) {

  auto wz=cx::visRect();

//  _wallTiles= 20;
//  _roofTiles=5;
//  _chimneys=5;

  _tileHeight = wz.size.height / TILE_H_SIZE;
  _tileWidth = wz.size.width / TILE_W_SIZE;
  _puffing = false;

  CC_HIDE(sprite);
}

//////////////////////////////////////////////////////////////////////////////
//
Block* Block::create () {
  auto s=cx::reifySprite("blank.png");
  auto block = mc_new1(Block,s);
  block->initBlock();
  return block;
}

//////////////////////////////////////////////////////////////////////////////
//
void Block::setPuffing (bool value) {

  _puffing = value;

  if (value) {
    sprite->runAction(_puffSpawn->clone());
    auto hide = c::Sequence::create(
        c::DelayTime::create(2.5f),
        c::CallFunc::create([=](){
          this->hidePuffs();
          }),
        CC_NIL);
    sprite->runAction(hide);
  } else {
    //reset all puffs
    _puffIndex = 0;
    for (auto chimney : _chimneys) {
      for (auto j = 0; j < TOTAL_PUFFS; ++j) {
        auto puff = (c::Sprite*) chimney->getChildByTag(j);
        CC_HIDE(puff);
        puff->stopAllActions();
        puff->setScale(1.0);
        puff->setOpacity(255);
        puff->setPosition(0,0);
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Block::hidePuffs() {
  setPuffing(false);
}

//////////////////////////////////////////////////////////////////////////////
//
void Block::setupBlock(int width, int height, int type) {

  this->setPuffing(false);

  _type = type;
  _width = width * _tileWidth;
  _height = height * _tileHeight + _tileHeight * 0.49f;
  sprite->setPositionY(_height);

    c::SpriteFrame *wallFrame;
    c::SpriteFrame *roofFrame = rand() % 10 > 6 ? _roof1 : _roof2;

  float chimneyX[] = {0,0,0,0,0};
  int num_chimneys;

  switch (type) {

    case kBlockGap:
      CC_HIDE(sprite);
    return;

    case kBlock1:
      wallFrame = _tile1;
      chimneyX[0] = 0.2f;
      chimneyX[1] = 0.8f;
      num_chimneys = 2;
    break;

    case kBlock2:
      wallFrame = _tile2;
      chimneyX[0] = 0.2f;
      chimneyX[1] = 0.8f;
      chimneyX[2] = 0.5f;
      num_chimneys = 3;
    break;

    case kBlock3:
      wallFrame = _tile3;
      chimneyX[0] = 0.2f;
      chimneyX[1] = 0.8f;
      chimneyX[2] = 0.5f;
      num_chimneys = 3;
    break;

    case kBlock4:
      wallFrame = _tile4;
      chimneyX[0] = 0.2f;
      chimneyX[1] = 0.5f;
      num_chimneys = 2;
    break;
  }

  for (auto i=0; i < _chimneys.size(); ++i) {
    auto chimney = _chimneys.at(i);
    if (i < num_chimneys) {
      chimney->setPosition(chimneyX[i] * _width, 0);
      CC_SHOW(chimney);
    } else {
      CC_HIDE(chimney);
    }
  }

  CC_SHOW(sprite);

  for (auto tile : _roofTiles) {
    if (tile->getPositionX() < _width) {
      CC_SHOW(tile);
      tile->setDisplayFrame(roofFrame);
    } else {
      CC_HIDE(tile);
    }
  }

  for (auto tile : _wallTiles) {
    if (tile->getPositionX() < _width &&
        tile->getPositionY() > -_height) {
      CC_SHOW(tile);
      tile->setDisplayFrame(wallFrame);
    } else {
      CC_HIDE(tile);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Block::initBlock() {

  _tile1 = cx::getSpriteFrame("building_1.png");
  _tile2 = cx::getSpriteFrame("building_2.png");
  _tile3 = cx::getSpriteFrame("building_3.png");
  _tile4 = cx::getSpriteFrame("building_4.png");

  _roof1 = cx::getSpriteFrame("roof_1.png");
  _roof2 = cx::getSpriteFrame("roof_2.png");

  //create tiles
  for (auto i = 0; i < 5; ++i) {
    auto tile = cx::reifySprite("roof_1.png");
    tile->setAnchorPoint(cx::anchorTL());
    tile->setPosition(i*_tileWidth, 0);
    CC_HIDE(tile);
    sprite->addChild(tile, kMiddleground, kRoofTile);
    _roofTiles.pushBack(tile);

    for (auto j = 0; j < 4; ++j) {
      tile = cx::reifySprite("building_1.png");
      tile->setAnchorPoint(cx::anchorTL());
      tile->setPosition(
          i*_tileWidth, -1 * (_tileHeight*0.47f + j*_tileHeight));
      CC_HIDE(tile);
      sprite->addChild(tile, kBackground, kWallTile);
      _wallTiles.pushBack(tile);
    }
  }

  for (auto i = 0; i < 5; ++i) {
    auto chimney = cx::reifySprite("chimney.png");
    CC_HIDE(chimney);
    sprite->addChild(chimney, kForeground, kChimney);
    _chimneys.pushBack(chimney);

    for (auto j = 0; j < TOTAL_PUFFS; ++j) {
      auto puff = cx::reifySprite("puff_1.png");
        puff->setAnchorPoint(c::Vec2(0,-0.5));
      CC_HIDE(puff);
      chimney->addChild(puff, -1, j);
    }
  }

    auto animation = c::Animation::create();
    c::SpriteFrame *frame;

  for (auto i = 1; i <= 4; ++i) {
    frame = cx::getSpriteFrame("puff_"+s::to_string(i)+".png");
    animation->addSpriteFrame(frame);
  }

  animation->setDelayPerUnit(0.75f / 4.0f);
  animation->setRestoreOriginalFrame(false);
  animation->setLoops(-1);
    _puffAnimation = c::Animate::create(animation);
  _puffAnimation->retain();

  _puffSpawn = c::Repeat::create(
      c::Sequence::create(c::DelayTime::create(0.5f),
        c::CallFunc::create(
          [=](){ this->createPuff(); }),
        CC_NIL),
      TOTAL_PUFFS);
  _puffSpawn->retain();

  _puffMove = c::MoveBy::create(1.0f, c::Vec2(-100,80));
  _puffMove->retain();
  _puffFade = c::FadeOut::create(2.0f);
  _puffFade->retain();
  _puffScale = c::ScaleBy::create(1.5f, 1.5);
  _puffScale->retain();

  _puffIndex = 0;
}

//////////////////////////////////////////////////////////////////////////////
//
void Block::createPuff() {

  for (auto chimney : _chimneys) {
    if (chimney->isVisible()) {
      auto puff = chimney->getChildByTag(_puffIndex);
      CC_SHOW(puff);
      puff->stopAllActions();
      puff->setScale(1.0);
      puff->setOpacity(255);
      puff->setPosition(0,0);
      puff->runAction( _puffAnimation->clone());
      puff->runAction( _puffMove->clone());
      puff->runAction( _puffScale->clone());
    }
  }

  ++_puffIndex;
  if (_puffIndex == TOTAL_PUFFS) {
    _puffIndex = 0;
  }
}




NS_END



