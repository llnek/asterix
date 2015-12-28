// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2015, Ken Leung. All rights reserved.

#include "audio/include/SimpleAudioEngine.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventListenerMouse.h"
#include "base/CCEventListenerTouch.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "XScene.h"
#include "GameScene.h"

NS_ALIAS(den, CocosDenshion)
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
GameLayer::~GameLayer() {
  mc_del_ptr(engine)
}

//////////////////////////////////////////////////////////////////////////////
//
GameLayer::GameLayer() {
}

//////////////////////////////////////////////////////////////////////////////
//
bool GameLayer::keyPoll(KEYCODE key) {
  int k= (int) key;
  return k >= 0 && k < 256 ? keys[k] : false;
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::disableListeners() {
  //CCLOG("disabling event handlers");
  try { if (cx::isDesktop()) setKeyboardEnabled(false); } catch (...) { }
  try { if (cx::isDesktop()) setMouseEnabled(false); } catch (...) { }
  try { if (!cx::isDesktop()) setTouchEnabled(false); } catch (...) { }
  keys.fill(false);
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::enableListeners() {
  disableListeners();
  try {
    if (cx::isDesktop())
    setMouseEnabled(true);
  } catch (...) {
    CCLOG("failed to init-mouse");
  }
  try {
    if (cx::isDesktop())
    setKeyboardEnabled(true);
  } catch (...) {
    CCLOG("failed to init-keys");
  }
  try {
    if (!cx::isDesktop())
    setTouchEnabled(true);
  } catch (...) {
    CCLOG("failed to init-touch");
  }
  //CCLOG("enabled event handlers");
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::onKeyPressed(KEYCODE k, c::Event*) {
  int n= (int)k;
  if (n >= 0 && n < 256) {
    this->keys[n]= true;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::onKeyReleased(KEYCODE k, c::Event*) {
  int n= (int)k;
  if (n >= 0 && n < 256) {
    this->keys[n]=false;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::onMouseDown(c::Event*) {
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::onMouseUp(c::Event* ) {
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::onMouseMove(c::Event*) {
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::onMouseScroll(c::Event*) {
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::setMouseEnabled(bool enabled) {
  if (_mouseEnabled != enabled) {
    _mouseEnabled = enabled;
    if (enabled) {
      if (NNP(_mouseListener)) { return; }
      auto n = c::EventListenerMouse::create();
      _mouseListener = n;
      n->onMouseScroll = CC_CALLBACK_1(GameLayer::onMouseScroll, this);
      n->onMouseMove = CC_CALLBACK_1(GameLayer::onMouseMove, this);
      n->onMouseUp = CC_CALLBACK_1(GameLayer::onMouseUp, this);
      n->onMouseDown = CC_CALLBACK_1(GameLayer::onMouseDown, this);
      _eventDispatcher->addEventListenerWithSceneGraphPriority(n, this);
    }
    else {
      _eventDispatcher->removeEventListener(_mouseListener);
      SNPTR(_mouseListener)
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void GameLayer::update(float dt) {
  if (!cx::isTransitioning() &&
      MGMS()->isLive() &&
      NNP(engine)) {
    engine->update(dt);
  }
}

NS_END(fusii)

