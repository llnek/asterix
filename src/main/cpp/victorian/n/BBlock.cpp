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
  CC_DROP(_puffAnimation);
  CC_DROP(_puffSpawn);
  CC_DROP(_puffMove);
  CC_DROP(_puffFade);
  CC_DROP(_puffScale);
  _wallTiles.clear();
  _chimneys.clear();
  _roofTiles.clear();
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
void Block::setup(const CCT_SZ &sz, int type) {

  auto wz= cx::visSize();
  auto th = wz.height / TILE_H_SIZE;
  auto tw = wz.width / TILE_W_SIZE;

  auto h= sz.height * th + th * 0.49;
  auto w= sz.width * tw;

  this->setPuffing(false);
  _type = type;
  setPositionY(h);

  auto roof= cx::randInt(10) > 6 ? _roof1 : _roof2;
  float chimneyX[] = {0,0,0,0,0};
  int num_chimneys;
  auto wall=roof;

  switch (type) {

    case kBlockGap:
      CC_HIDE(this);
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
      ch->setPosition(chimneyX[i] * w, 0);
      CC_SHOW(ch);
    } else {
      CC_HIDE(ch);
    }
  }

  CC_SHOW(this);

  for (auto tile : _roofTiles) {
    if (tile->getPositionX() < w) {
      CC_SHOW(tile);
      tile->setDisplayFrame(roof);
    } else {
      CC_HIDE(tile);
    }
  }

  for (auto tile : _wallTiles) {
    if (tile->getPositionX() < w &&
        tile->getPositionY() > -h) {
      CC_SHOW(tile);
      tile->setDisplayFrame(wall);
    } else {
      CC_HIDE(tile);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
bool Block::initWithSpriteFrameName(const sstr &fn) {

  auto rc= c::Sprite::initWithSpriteFrameName(fn);
  if (!rc) { return false; }
  auto wz= cx::visSize();
  auto th = wz.height / TILE_H_SIZE;
  auto tw = wz.width / TILE_W_SIZE;

  _tile4 = cx::getSpriteFrame("building_4.png");
  _tile3 = cx::getSpriteFrame("building_3.png");
  _tile2 = cx::getSpriteFrame("building_2.png");
  _tile1 = cx::getSpriteFrame("building_1.png");
  _roof2 = cx::getSpriteFrame("roof_2.png");
  _roof1 = cx::getSpriteFrame("roof_1.png");

  //create tiles
  for (auto i = 0; i < 5; ++i) {
    auto tile = cx::reifySprite("roof_1.png");
    tile->setAnchorPoint(cx::anchorTL());
    tile->setPosition(i*tw, 0);
    CC_HIDE(tile);
    this->addChild(tile, kMiddleground, kRoofTile);
    _roofTiles.pushBack(tile);

    for (auto j = 0; j < 4; ++j) {
      auto tile = cx::reifySprite("building_1.png");
      tile->setAnchorPoint(cx::anchorTL());
      tile->setPosition(i*tw, -1 * (th*0.47 + j*th));
      CC_HIDE(tile);
      this->addChild(tile, kBackground, kWallTile);
      _wallTiles.pushBack(tile);
    }
  }

  for (auto i = 0; i < 5; ++i) {
    auto ch= cx::reifySprite("chimney.png");
    CC_HIDE(ch);
    this->addChild(ch, kForeground, kChimney);
    _chimneys.pushBack(ch);

    for (auto j = 0; j < TOTAL_PUFFS; ++j) {
      auto puff = cx::reifySprite("puff_1.png");
      puff->setAnchorPoint(CCT_PT(0,-0.5));
      CC_HIDE(puff);
      ch->addChild(puff, -1, j);
    }
  }

  auto anim= c::Animation::create();
  anim->setRestoreOriginalFrame(false);
  anim->setDelayPerUnit(0.75 / 4.0);
  anim->setLoops(-1);
  for (auto i = 1; i <= 4; ++i) {
    auto f= cx::getSpriteFrame("puff_"+FTOS(i)+".png");
    anim->addSpriteFrame(f);
  }
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

  _puffMove = c::MoveBy::create(1, CCT_PT(-100,80));
  _puffFade = c::FadeOut::create(2);
  _puffScale = c::ScaleBy::create(1.5, 1.5);
  CC_KEEP(_puffMove);
  CC_KEEP(_puffFade);
  CC_KEEP(_puffScale);

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
          c::CallFunc::create([=](){
            this->setPuffing(false);
            }),
          CC_NIL));
  } else {
    //reset all puffs
    _puffIndex = 0;
    for (auto ch : _chimneys) {
      for (auto j = 0; j < TOTAL_PUFFS; ++j) {
        auto puff = (c::Sprite*) ch->getChildByTag(j);
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
void Block::createPuff() {

  for (auto ch : _chimneys) {
    if (ch->isVisible()) {
      auto puff = ch->getChildByTag(_puffIndex);
      CC_SHOW(puff);
      puff->stopAllActions();
      puff->setScale(1);
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



