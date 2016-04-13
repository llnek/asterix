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

#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"
#include "BBlock.h"
#include "C.h"
#define TOTAL_PUFFS 3

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(victorian)

//////////////////////////////////////////////////////////////////////////////
//
Block::~Block() {
  CC_DROP(_puffAnimation);
  CC_DROP(_puffSpawn);
  CC_DROP(_puffMove);
  CC_DROP(_puffFade);
  CC_DROP(_puffScale);
  _wallTiles.clear();
  _roofTiles.clear();
  _chimneys.clear();
}

//////////////////////////////////////////////////////////////////////////////
//
owner<Block*> Block::create() {
  auto b= mc_new(Block);
  b->initWithSpriteFrameName("blank.png");
  b->autorelease();
  return b;
}

//////////////////////////////////////////////////////////////////////////////
//
bool Block::initWithSpriteFrameName(const sstr &fn) {
  auto rc= c::Sprite::initWithSpriteFrameName(fn);
  auto wz= cx::visSize();
  if (!rc) { return rc; }
  _tile4= cx::getSpriteFrame("building_4.png");
  _tile3= cx::getSpriteFrame("building_3.png");
  _tile2= cx::getSpriteFrame("building_2.png");
  _tile1= cx::getSpriteFrame("building_1.png");
  _roof2= cx::getSpriteFrame("roof_2.png");
  _roof1= cx::getSpriteFrame("roof_1.png");
  _tileH= wz.height / TILE_H_SIZE;
  _tileW= wz.width / TILE_W_SIZE;

  //create tiles
  for (auto i = 0; i < 5; ++i) {
    auto tile= cx::reifySprite("roof_1.png");
    tile->setAnchorPoint(cx::anchorTL());
    tile->setPosition(i*_tileW, 0);
    CC_HIDE(tile);
    this->addChild(tile, kMiddleground, kRoofTile);
    _roofTiles.pushBack(tile);

    for (auto j = 0; j < 4; ++j) {
      tile = cx::reifySprite("building_1.png");
      tile->setAnchorPoint(cx::anchorTL());
      tile->setPosition(i * _tileW, -_tileH * (0.47 + j));
      CC_HIDE(tile);
      this->addChild(tile, kBackground, kWallTile);
      _wallTiles.pushBack(tile);
    }
  }

  for (auto i = 0; i < 5; ++i) {
    auto chimney = cx::reifySprite("chimney.png");
    CC_HIDE(chimney);
    this->addChild(chimney, kForeground, kChimney);
    _chimneys.pushBack(chimney);
    for (auto j = 0; j < TOTAL_PUFFS; ++j) {
      auto puff = cx::reifySprite("puff_1.png");
      puff->setAnchorPoint(CCT_PT(0,-0.5));
      CC_HIDE(puff);
      chimney->addChild(puff, -1, j);
    }
  }

  auto animation= c::Animation::create();
  animation->setRestoreOriginalFrame(false);
  animation->setDelayPerUnit(0.75 / 4.0);
  animation->setLoops(-1);
  for (auto i = 1; i <= 4; ++i) {
    animation->addSpriteFrame(
      cx::getSpriteFrame("puff_" + FTOS(i) + ".png"));
  }
  _puffAnimation = c::Animate::create(animation);
  CC_KEEP(_puffAnimation);

  _puffSpawn = c::Repeat::create(
      c::Sequence::create(
        c::DelayTime::create(0.5),
        c::CallFunc::create([=]() {
           this->createPuff();
          }),
        CC_NIL),
      TOTAL_PUFFS);
  CC_KEEP(_puffSpawn);

  _puffMove = c::MoveBy::create(1, CCT_PT(-100,80));
  _puffFade = c::FadeOut::create(2);
  _puffScale = c::ScaleBy::create(1.5, 1.5);
  CC_KEEP(_puffScale);
  CC_KEEP(_puffMove);
  CC_KEEP(_puffFade);
  _puffIndex = 0;
}

//////////////////////////////////////////////////////////////////////////////
//
void Block::setPuffing(bool value) {
  _puffing = value;
  if (value) {
    this->runAction(_puffSpawn->clone());
    this->runAction(
        c::Sequence::create(
          c::DelayTime::create(2.5),
          c::CallFunc::create([=]() {
            this->setPuffing(false);
            }),
          CC_NIL));
  } else {
    //reset all puffs
    _puffIndex = 0;
    for (auto chimney : _chimneys) {
      for (auto j = 0; j < TOTAL_PUFFS; ++j) {
        auto puff = (c::Sprite*) chimney->getChildByTag(j);
        CC_HIDE(puff);
        puff->stopAllActions();
        puff->setScale(1);
        puff->setOpacity(255);
        puff->setPosition(0,0);
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Block::setup(float width, float height, int type) {

  auto roofFrame = cx::randInt(10) > 6 ? _roof1 : _roof2;
  c::SpriteFrame *wallFrame;
  int num_chimneys;
  float chimneyX[] = {0,0,0,0,0};

  _height = (0.49 + height) * _tileH;
  _width = width * _tileW;
  _type = type;

  this->setPositionY(_height);
  this->setPuffing(false);

  switch (type) {

    case kBlockGap:
      CC_HIDE(this);
    return;

    case kBlock1:
      wallFrame = _tile1;
      chimneyX[0] = 0.2;
      chimneyX[1] = 0.8;
      num_chimneys = 2;
    break;

    case kBlock2:
      wallFrame = _tile2;
      chimneyX[0] = 0.2;
      chimneyX[1] = 0.8;
      chimneyX[2] = 0.5;
      num_chimneys = 3;
    break;

    case kBlock3:
      wallFrame = _tile3;
      chimneyX[0] = 0.2;
      chimneyX[1] = 0.8;
      chimneyX[2] = 0.5;
      num_chimneys = 3;
    break;

    case kBlock4:
      wallFrame = _tile4;
      chimneyX[0] = 0.2;
      chimneyX[1] = 0.5;
      num_chimneys = 2;
    break;
  }

  for (auto i = 0; i < _chimneys.size(); ++i) {
    auto chimney = _chimneys.at(i);
    if (i < num_chimneys) {
      chimney->setPosition(chimneyX[i] * _width, 0);
      CC_SHOW(chimney);
    } else {
      CC_HIDE(chimney);
    }
  }

  CC_SHOW(this);

  for (auto tile : _roofTiles) {
    if (tile->getPositionX() < _width) {
      tile->setDisplayFrame(roofFrame);
      CC_SHOW(tile);
    } else {
      CC_HIDE(tile);
    }
  }

  for (auto tile : _wallTiles) {
    if (tile->getPositionX() < _width &&
        tile->getPositionY() > -_height) {
      tile->setDisplayFrame(wallFrame);
      CC_SHOW(tile);
    } else {
      CC_HIDE(tile);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Block::createPuff () {

  for (auto chimney : _chimneys) {
    if (chimney->isVisible()) {
      auto puff = chimney->getChildByTag(_puffIndex);
      CC_SHOW(puff);
      puff->stopAllActions();
      puff->setScale(1);
      puff->setOpacity(255);
      puff->setPosition(0,0);
      puff->runAction(_puffAnimation->clone());
      puff->runAction(_puffMove->clone());
      puff->runAction(_puffScale->clone());
    }
  }

  if (++_puffIndex == TOTAL_PUFFS) { _puffIndex = 0; }

}


NS_END



