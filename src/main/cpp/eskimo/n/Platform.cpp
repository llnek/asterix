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
#include "core/CCSX.h"
#include "lib.h"
#include "Platform.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(eskimo)

//////////////////////////////////////////////////////////////////////////////
//
void PlatformSprite::bind(GVars *ss) {

  //create body
  b2BodyDef bodyDef; bodyDef.type = b2_staticBody;
  _body = ss->world->CreateBody(&bodyDef);
  _body->SetSleepingAllowed(true);
  _body->SetUserData(this);

  //register game notifications
  auto onGravityChanged = [=] (c::EventCustom*) {
    if (this->isVisible()) {
      switchTexture();
    }
  };

  ADD_NOTIFICATION(this, NOTIFY_GSWITCH, onGravityChanged);
  this->ss = ss;
}

//////////////////////////////////////////////////////////////////////////////
//
PlatformSprite::~PlatformSprite() {
  EVENT_DISPATCHER->removeCustomEventListeners(NOTIFY_GSWITCH);
}

//////////////////////////////////////////////////////////////////////////////
//
PlatformSprite* PlatformSprite::create(GVars *ss) {
  auto sprite = mc_new(PlatformSprite);
  sprite->initWithSpriteFrameName("blank.png");
  sprite->autorelease();
  sprite->bind(ss);
  sprite->createTiles();
  //CC_HIDE(sprite);
  return sprite;
}

//////////////////////////////////////////////////////////////////////////////
//
void PlatformSprite::initPlatform(int width, float angle, const c::Vec2 &position) {

  //Define fixture
  //Define shape
  b2PolygonShape box; box.SetAsBox(HTV(width)/PTM_RATIO,
      HTV(PLATFORM_HEIGHT) / PTM_RATIO);
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &box;
  fixtureDef.density = 1;
  fixtureDef.restitution = 0;

  //reutilize body from the pool: so destroy any existent fixture
  auto tmp= _body->GetFixtureList();
  if (tmp) {
    _body->DestroyFixture(tmp); }

  _body->CreateFixture(&fixtureDef);
  _body->SetTransform(
      b2Vec2(position.x / PTM_RATIO, position.y / PTM_RATIO),
      CC_DEGREES_TO_RADIANS(-angle));
  _body->SetActive(true);

  //set unused tiles in the platform invisible
  auto startX = -HTV(width) + HTV(TILE);
  int i=0;
  F__LOOP(it,_tiles) {
    auto block = *it;
    if (i >= width/TILE) { CC_HIDE(block); } else {
      CC_SHOW(block);
      block->setPosition(startX + i * TILE, 0);
      block->setFlippedX(false);
      block->setFlippedY(false);
      if ((int)angle == 0) {
        if (position.y >= TILE * 13) block->setFlippedY(true);
      }
      if ((int)angle == 90) {
        if (position.x > TILE * 5) block->setFlippedX(true);
      }
    }
    ++i;
  }
  switchTexture();

  this->setPosition(position);
  this->setRotation(angle);
  setVisible(true);
}

//////////////////////////////////////////////////////////////////////////////
//
//texture platform with tiles
void PlatformSprite::switchTexture() {
  F__LOOP(it, _tiles) {
    auto block= *it;
    if (block->isVisible()) {
      block->setSpriteFrame(
          cx::getSpriteFrame( fmtPng("block_",ss->gravity)));
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
//create platform with maximum number of tiles a platform can have (larger side / tile size).
void PlatformSprite::createTiles() {
  for (auto i = 0; i < TILES_PER_PLATFORM; ++i) {
    auto block = cx::reifySprite("block_1.png");
    CC_HIDE(block);
    _tiles.push_back(block);
    this->addChild(block, kBackground, kSpriteBlock);
  }
}


NS_END


