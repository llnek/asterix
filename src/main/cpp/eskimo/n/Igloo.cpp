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
#include "Igloo.h"
NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(eskimo)

//////////////////////////////////////////////////////////////////////////////
//
Igloo::~Igloo() {
  CC_EDISP()->removeCustomEventListeners(NOTIFY_LEVEL_DONE);
  CC_EDISP()->removeCustomEventListeners(NOTIFY_GSWITCH);
}

//////////////////////////////////////////////////////////////////////////////
//
Igloo::Igloo(GVars *ss) {

  auto onGravityChanged = [=](c::EventCustom*) {
    _block->setSpriteFrame(
        cx::getSpriteFrame(
          fmtPng("block_large_",ss->gravity)));
  };

  auto onLevelCompleted = [=](c::EventCustom*) {
    setDisplayFrame(cx::getSpriteFrame("igloo_on.png"));
  };

    //register game notifications
  CC_ADD_ELC(this, NOTIFY_LEVEL_DONE, onLevelCompleted);
  CC_ADD_ELC(this, NOTIFY_GSWITCH, onGravityChanged);
}

//////////////////////////////////////////////////////////////////////////////
//
owner<Igloo*> Igloo::create(not_null<GVars*> ss) {
  auto sprite = mc_new1(Igloo, ss);
  sprite->initWithSpriteFrameName("igloo_off.png");
  sprite->addBlock();
  sprite->autorelease();
  return sprite;
}

//////////////////////////////////////////////////////////////////////////////
//
void Igloo::initIgloo(int gravity, const CCT_PT &pos) {

  _block->setDisplayFrame(
      cx::getSpriteFrame(
        fmtPng("block_large_",gravity)));

  this->setDisplayFrame(cx::getSpriteFrame("igloo_off.png"));
  CC_POS1(this, pos);
  setVisible(true);
}

//////////////////////////////////////////////////////////////////////////////
//
void Igloo::addBlock() {
  _block = cx::reifySprite("block_large_1.png");
  _block->setAnchorPoint(cx::anchorBL());
  CC_POS2(_block, 0, -TILE * 0.75f);
  this->addChild(_block, kBackground);
}



NS_END


