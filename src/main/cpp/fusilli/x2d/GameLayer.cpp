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
#include "GameLayer.h"

NS_ALIAS(den, CocosDenshion)
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
GameLayer::~GameLayer() {
  //SNPTR(options)
  //SNPTR(engine)
}

//////////////////////////////////////////////////////////////////////////////
//
GameLayer::GameLayer() {
  SNPTR(options)
  SNPTR(mouse)
  SNPTR(keys)
  SNPTR(touch)
  SNPTR(engine)
}

//////////////////////////////////////////////////////////////////////////////
//
bool GameLayer::keyPoll(c::EventKeyboard::KeyCode key) {
  int k= (int) key;
  return k >= 0 && k < 256 ? keyboard[k] : false;
}

//////////////////////////////////////////////////////////////////////////////
//
const Box4 GameLayer::getEnclosureBox() {
  return cx::visBox();
}

/*
//////////////////////////////////////////////////////////////////////////////
//
XLayer* GameLayer::realize() {
  enableEventHandlers();
  cx::pauseAudio();
  inizGame();
  this->scheduleUpdate();
  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::finzGame() {
  disableEventHandlers();
  cx::pauseAudio();
  onGameOver();
  this->unscheduleUpdate();
}
*/

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::addListener(c::EventListener* ln) {
  _eventDispatcher->addEventListenerWithSceneGraphPriority(ln, this);
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::disableListeners() {
  CCLOG("disabling event handlers");

  if (NNP(mouse)) {
    _eventDispatcher->removeEventListener(mouse);
  }
  if (NNP(keys)) {
    _eventDispatcher->removeEventListener(keys);
  }
  if (NNP(touch)) {
    _eventDispatcher->removeEventListener(touch);
  }

  SNPTR(mouse)
  SNPTR(keys)
  SNPTR(touch)

  keyboard.fill(false);
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::enableListeners() {
  disableListeners();
  try {
    initMouse();
  } catch (...) {
    CCLOG("failed to init-mouse");
  }
  try {
    initKeys();
  } catch (...) {
    CCLOG("failed to init-keys");
  }
  try {
    initTouch();
  } catch (...) {
    CCLOG("failed to init-touch");
  }
  CCLOG("enabled event handlers");
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::initMouse() {
  //m->onMouseScroll = CC_CALLBACK_1(GameLayer::onMouseScroll, this);
  //m->onMouseMove = CC_CALLBACK_1(GameLayer::onMouseMove, this);
  //m->onMouseDown = CC_CALLBACK_1(GameLayer::onMouseDown, this);
  //m->onMouseUp = CC_CALLBACK_1(GameLayer::onMouseUp, this);
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::initKeys() {

  keys = c::EventListenerKeyboard::create();
  addListener(keys);

  keys->onKeyReleased = [=] (c::EventKeyboard::KeyCode k, c::Event*) {
    int n= (int)k;
    if (n >= 0 && n < 256) { this->keyboard[n]=false; }
  };
  keys->onKeyPressed = [=] (c::EventKeyboard::KeyCode k, c::Event*) {
    int n= (int)k;
    if (n >= 0 && n < 256) { this->keyboard[n]= true; }
  };

  CCLOG("init-keys: listener = %p", keys);
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::initTouch() {
  //  Reify a "one by one" touch event listener
  // (processes one touch at a time)
  //auto t= c::EventListenerTouchOneByOne::create();
  //_eventDispatcher->addEventListenerWithSceneGraphPriority(t, this);
  //touch =t;

  // trigger when you push down
  //t->onTouchBegan = [](c::Touch* t, c::Event* e){ return true; };
  // trigger when moving touch
  //t->onTouchMoved = [](c::Touch* t, c::Event* e){ }
  // trigger when you let up
  //t->onTouchEnded = [=](c::Touch* t, c::Event* e){ };

  // When "swallow touches" is true, then returning 'true' from the
  // onTouchBegan method will "swallow" the touch event, preventing
  // other listeners from using it.
  //t->setSwallowTouches(true);
}

//////////////////////////////////////////////////////////////////////////////
//
/*
void GameLayer::update(float dt) {
  if (getSceneX()->isLive() &&
      NNP(engine)) {
    engine->update(dt);
  }
}
*/


NS_END(fusii)

