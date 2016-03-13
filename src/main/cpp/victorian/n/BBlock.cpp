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
Block::Block (not_null<c::Node*> s)
  : Widget(s) {

  auto wz=cx::visRect();
  CC_HIDE(s);
//  _wallTiles= 20;
//  _roofTiles=5;
//  _chimneys=5;
  _tileHeight = wz.size.height / TILE_H_SIZE;
  _tileWidth = wz.size.width / TILE_W_SIZE;
  _puffing = false;
}

//////////////////////////////////////////////////////////////////////////////
//
Block* Block::create () {
  auto s= cx::reifySprite("blank.png");
  auto block = mc_new1(Block,s);
  block->initBlock();
  return block;
}

//////////////////////////////////////////////////////////////////////////////
//
void Block::setPuffing (bool value) {

  _puffing = value;

  if (value) {
    node->runAction(_puffSpawn->clone());
    auto hide = c::Sequence::create(
        c::DelayTime::create(2.5),
        c::CallFunc::create([=](){
          this->hidePuffs();
          }),
        CC_NIL);
    node->runAction(hide);
  } else {
    //reset all puffs
    _puffIndex = 0;
    for (auto ch : _chimneys) {
      for (auto j = 0; j < TOTAL_PUFFS; ++j) {
        auto puff = (c::Sprite*) ch->getChildByTag(j);
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
void Block::setupBlock(int w, int h, int type) {

  this->setPuffing(false);

  _height = h * _tileHeight + _tileHeight * 0.49;
  _width = w * _tileWidth;
  _type = type;
  node->setPositionY(_height);

  auto roof= rand() % 10 > 6 ? _roof1 : _roof2;
  float chimneyX[] = {0,0,0,0,0};
  int num_chimneys;
  auto wall=roof;

  switch (type) {

    case kBlockGap:
      CC_HIDE(node);
    return;

    case kBlock1:
      chimneyX[0] = 0.2;
      chimneyX[1] = 0.8;
      num_chimneys = 2;
      wall= _tile1;
    break;

    case kBlock2:
      chimneyX[0] = 0.2;
      chimneyX[1] = 0.8;
      chimneyX[2] = 0.5;
      num_chimneys = 3;
      wall= _tile2;
    break;

    case kBlock3:
      chimneyX[0] = 0.2;
      chimneyX[1] = 0.8;
      chimneyX[2] = 0.5;
      num_chimneys = 3;
      wall= _tile3;
    break;

    case kBlock4:
      chimneyX[0] = 0.2;
      chimneyX[1] = 0.5;
      num_chimneys = 2;
      wall= _tile4;
    break;
  }

  for (auto i=0; i < _chimneys.size(); ++i) {
    auto ch= _chimneys.at(i);
    if (i < num_chimneys) {
      ch->setPosition(chimneyX[i] * _width, 0);
      CC_SHOW(ch);
    } else {
      CC_HIDE(ch);
    }
  }

  CC_SHOW(node);

  for (auto tile : _roofTiles) {
    if (tile->getPositionX() < _width) {
      CC_SHOW(tile);
      tile->setDisplayFrame(roof);
    } else {
      CC_HIDE(tile);
    }
  }

  for (auto tile : _wallTiles) {
    if (tile->getPositionX() < _width &&
        tile->getPositionY() > -_height) {
      CC_SHOW(tile);
      tile->setDisplayFrame(wall);
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
    node->addChild(tile, kMiddleground, kRoofTile);
    _roofTiles.pushBack(tile);

    for (auto j = 0; j < 4; ++j) {
      tile = cx::reifySprite("building_1.png");
      tile->setAnchorPoint(cx::anchorTL());
      tile->setPosition(
          i*_tileWidth, -1 * (_tileHeight*0.47 + j*_tileHeight));
      CC_HIDE(tile);
      node->addChild(tile, kBackground, kWallTile);
      _wallTiles.pushBack(tile);
    }
  }

  for (auto i = 0; i < 5; ++i) {
    auto ch= cx::reifySprite("chimney.png");
    CC_HIDE(ch);
    node->addChild(ch, kForeground, kChimney);
    _chimneys.pushBack(ch);

    for (auto j = 0; j < TOTAL_PUFFS; ++j) {
      auto puff = cx::reifySprite("puff_1.png");
      puff->setAnchorPoint(c::Vec2(0,-0.5));
      CC_HIDE(puff);
      ch->addChild(puff, -1, j);
    }
  }

  auto anim= c::Animation::create();
  for (auto i = 1; i <= 4; ++i) {
    auto f= cx::getSpriteFrame("puff_"+s::to_string(i)+".png");
    anim->addSpriteFrame(f);
  }

  anim->setRestoreOriginalFrame(false);
  anim->setDelayPerUnit(0.75 / 4.0);
  anim->setLoops(-1);
  _puffAnimation = c::Animate::create(anim);
  CC_KEEP(_puffAnimation);

  _puffSpawn = c::Repeat::create(
      c::Sequence::create(
        c::DelayTime::create(0.5),
        c::CallFunc::create(
          [=](){ this->createPuff(); }),
        CC_NIL),
      TOTAL_PUFFS);
  CC_KEEP(_puffSpawn);

  _puffMove = c::MoveBy::create(1.0, c::Vec2(-100,80));
  _puffFade = c::FadeOut::create(2.0);
  _puffScale = c::ScaleBy::create(1.5, 1.5);
  CC_KEEP(_puffMove);
  CC_KEEP(_puffFade);
  CC_KEEP(_puffScale);

  _puffIndex = 0;
}

//////////////////////////////////////////////////////////////////////////////
//
void Block::createPuff() {

  for (auto ch : _chimneys) {
    if (ch->isVisible()) {
      auto puff = ch->getChildByTag(_puffIndex);
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



