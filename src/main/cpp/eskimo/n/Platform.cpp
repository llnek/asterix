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
#include "lib.h"
#include "Platform.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(eskimo)

//////////////////////////////////////////////////////////////////////////////
//
Platform::Platform (GVars *ss) {

  //create body
  b2BodyDef bodyDef;
  bodyDef.type = b2_staticBody;

  _body = ss->world->CreateBody(&bodyDef);
  _body->SetSleepingAllowed(true);
  _body->SetUserData(this);

  //register game notifications
  auto onGravityChanged = [=] (EventCustom*) {
    if (this->isVisible()) {
      switchTexture();
    }
  };

  ADD_NOTIFICATION(this, NOTIFICATION_GRAVITY_SWITCH, onGravityChanged);
}

//////////////////////////////////////////////////////////////////////////////
//
Platform* Platform::create(GVars *ss) {

  auto sprite = mc_new1(Platform,ss);
  if (sprite->initWithSpriteFrameName("blank.png")) {
    sprite->autorelease();
    sprite->createTiles();
    return sprite;
  } else {
    CC_SAFE_DELETE(sprite);
    return CC_NIL;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Platform::initPlatform(int width, float angle, const c::Vec2 &position) {

  //reutilize body from the pool: so destroy any existent fixture
  if (_body->GetFixtureList()) {
    _body->DestroyFixture(_body->GetFixtureList());
  }

  //Define shape
  b2PolygonShape box;
  box.SetAsBox(width * 0.5f /PTM_RATIO, PLATFORM_HEIGHT * 0.5f / PTM_RATIO);

  //Define fixture
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &box;
  fixtureDef.density = 1;
  fixtureDef.restitution = 0;

  _body->CreateFixture(&fixtureDef);
  _body->SetTransform(b2Vec2(position.x / PTM_RATIO,
        position.y / PTM_RATIO), CC_DEGREES_TO_RADIANS(-angle));
  _body->SetActive(true);

  //set unused tiles in the platform invisible
  auto startX = -width * 0.5f + TILE * 0.5f;
  F__LOOP(it,_tiles) {
    auto block = *it;

    if (i >= width/TILE) {
      block->setVisible(false);
    } else {
      block->setVisible(true);
      block->setPosition(Vec2(startX + i * TILE, 0));
      block->setFlippedX(false);
      block->setFlippedY(false);

      if (angle == 0.0f) {
        if (position.y >= TILE * 13) block->setFlippedY(true);
      }
      if (angle == 90.0f) {
        if (position.x > TILE * 5) block->setFlippedX(true);
      }
    }
  }
  switchTexture();

  this->setPosition(position);
  this->setRotation(angle);
  setVisible(true);
}

//////////////////////////////////////////////////////////////////////////////
//
//texture platform with tiles
void Platform::switchTexture() {
  F__LOOP(it, _tiles) {
    auto block= *it;
    if (block->isVisible()) {
      block->setSpriteFrame(cx::getSpriteFrame( fmtGPng(ss->gravity)));
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
//create platform with maximum number of tiles a platform can have (larger side / tile size).
void Platform::createTiles() {
  for (auto i = 0; i < TILES_PER_PLATFORM; ++i) {
    auto block = cx::reifySprite("block_1.png");
    CC_HIDE(block);
    _tiles.push_back(block);
    this->addChild(block, kBackground, kSpriteBlock);
  }
}


NS_END


