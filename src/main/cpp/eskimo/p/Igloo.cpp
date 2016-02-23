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
#include "core/CCSX.h"
#include "Igloo.h"
NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(eskimo)

//////////////////////////////////////////////////////////////////////////////
//
Igloo::~Igloo() {
  EVENT_DISPATCHER->removeCustomEventListeners(NOTIFICATION_LEVEL_COMPLETED);
  EVENT_DISPATCHER->removeCustomEventListeners(NOTIFICATION_GRAVITY_SWITCH);
}

//////////////////////////////////////////////////////////////////////////////
//
Igloo::Igloo(GVars *ss) {

  auto onGravityChanged = [=](c::EventCustom*) {
    _block->setSpriteFrame(cx::getSpriteFrame(fmtGPng(ss->gravity)));
  };

  auto onLevelCompleted = [=](c::EventCustom*) {
    setDisplayFrame(cx::getSpriteFrame("igloo_on.png"));
  };

    //register game notifications
  ADD_NOTIFICATION(this, NOTIFICATION_LEVEL_COMPLETED, onLevelCompleted);
  ADD_NOTIFICATION(this, NOTIFICATION_GRAVITY_SWITCH, onGravityChanged);
}

//////////////////////////////////////////////////////////////////////////////
//
Igloo* Igloo::create(GVars *ss) {
  auto sprite = mc_new1(Igloo, ss);
  if (sprite->initWithSpriteFrameName("igloo_off.png")) {
    sprite->autorelease();
    sprite->addBlock();
    return sprite;
  } else {
    CC_SAFE_DELETE(sprite);
    return CC_NIL;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Igloo::initIgloo(int gravity, const c::Vec2 &pos) {

  _block->setDisplayFrame( cx::getSpriteFrame(fmtGPng(gravity)));
  this->setPosition(pos);
  this->setDisplayFrame(cx::getSpriteFrame("igloo_off.png"));
  setVisible(true);
}

//////////////////////////////////////////////////////////////////////////////
//
void Igloo::addBlock () {
  _block = cx::reifySprite("block_large_1.png");
  _block->setAnchorPoint(cx::anchorBL());
  _block->setPosition(0, -TILE * 0.75f);
  this->addChild(_block, kBackground);
}



NS_END


