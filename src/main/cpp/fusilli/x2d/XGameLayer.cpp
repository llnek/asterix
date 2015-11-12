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
#include "XGameLayer.h"

NS_ALIAS(den, CocosDenshion)
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
XGameLayer::~XGameLayer() {
  //SNPTR(options)
}

//////////////////////////////////////////////////////////////////////////////
//
XGameLayer::XGameLayer() {
  SNPTR(mouse)
  SNPTR(keys)
  SNPTR(touch)
  //SNPTR(engine)
  //SNPTR(options)
}

//////////////////////////////////////////////////////////////////////////////
//
bool XGameLayer::KeyPoll(c::EventKeyboard::KeyCode key) {
  int k= (int) key;
  return k >= 0 && k < 256 ? keyboard[k] : false;
}

//////////////////////////////////////////////////////////////////////////////
//
const Box4 XGameLayer::GetEnclosureBox() {
  return cx::VisBox();
}

/*
//////////////////////////////////////////////////////////////////////////////
//
XLayer* XGameLayer::Realize() {
  EnableEventHandlers();
  cx::PauseAudio();
  InizGame();
  this->scheduleUpdate();
  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
void XGameLayer::FinzGame() {
  DisableEventHandlers();
  cx::PauseAudio();
  OnGameOver();
  this->unscheduleUpdate();
}
*/

//////////////////////////////////////////////////////////////////////////////
//
void XGameLayer::DisableEventHandlers() {
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
void XGameLayer::EnableEventHandlers() {
  DisableEventHandlers();
  try {
    InitMouse();
  } catch (...) {
    CCLOG("failed to init-mouse");
  }
  try {
    InitKeys();
  } catch (...) {
    CCLOG("failed to init-keys");
  }
  try {
    InitTouch();
  } catch (...) {
    CCLOG("failed to init-touch");
  }
  CCLOG("enabled event handlers");
}

//////////////////////////////////////////////////////////////////////////////
//
void XGameLayer::InitMouse() {
  auto m = c::EventListenerMouse::create();
  mouse=m;

  m->onMouseMove = CC_CALLBACK_1(XGameLayer::OnMouseMove, this);
  m->onMouseUp = CC_CALLBACK_1(XGameLayer::OnMouseUp, this);
  m->onMouseDown = CC_CALLBACK_1(XGameLayer::OnMouseDown, this);
  m->onMouseScroll = CC_CALLBACK_1(XGameLayer::OnMouseScroll, this);

  _eventDispatcher->addEventListenerWithSceneGraphPriority(m, this);
  CCLOG("init-mouse: listener = %p", m);
}

//////////////////////////////////////////////////////////////////////////////
//
void XGameLayer::OnMouseMove(c::Event* e) {
  auto evt = (c::EventMouse*) e;
  //GEvent e(evt);
  //evt->getDelta();
  //evt->getLocation();
  //CCLOG("mouse moved");
}

//////////////////////////////////////////////////////////////////////////////
//
void XGameLayer::OnMouseDown(c::Event* e) {
  auto evt = (c::EventMouse*) e;
  //GEvent e(evt);

  //CCLOG("mouse down");
}


//////////////////////////////////////////////////////////////////////////////
//
void XGameLayer::OnMouseUp(c::Event* e) {
  auto evt = (c::EventMouse*) e;
  //GEvent e(evt);
 // CCLOG("mouse up");
}

//////////////////////////////////////////////////////////////////////////////
//
void XGameLayer::OnMouseScroll(c::Event* e) {
  auto evt = (c::EventMouse*) e;
  //GEvent e(evt);
}

//////////////////////////////////////////////////////////////////////////////
//
void XGameLayer::OnKeyReleased(c::EventKeyboard::KeyCode code, c::Event* ) {
  //CCLOG("key released = %d", (int)code);
  int n= (int)code;
  if ( n >= 0 && n < 256) {
    keyboard[n]=false;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void XGameLayer::OnKeyPressed(c::EventKeyboard::KeyCode code, c::Event* ) {
  //CCLOG("key pressed = %d", (int)code);
  int n= (int)code;
  if ( n >= 0 && n < 256) {
    keyboard[n]= true;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void XGameLayer::InitKeys() {

  auto k = c::EventListenerKeyboard::create();
  keys=k;

  k->onKeyReleased = CC_CALLBACK_2(XGameLayer::OnKeyReleased, this);
  k->onKeyPressed = CC_CALLBACK_2(XGameLayer::OnKeyPressed, this);

  _eventDispatcher->addEventListenerWithSceneGraphPriority(k, this);
  CCLOG("init-keys: listener = %p", k);
}

//////////////////////////////////////////////////////////////////////////////
//
void XGameLayer::InitTouch() {
  //  Reify a "one by one" touch event listener
  // (processes one touch at a time)
  auto t= c::EventListenerTouchOneByOne::create();
  touch =t;

  // trigger when you push down
  t->onTouchBegan = [](c::Touch* t, c::Event* e){

      return true; // if you are consuming it
  };

  // trigger when moving touch
  t->onTouchMoved = [](c::Touch* t, c::Event* e){
      // your code
  };

  // trigger when you let up
  t->onTouchEnded = [=](c::Touch* t, c::Event* e){
      // your code
  };

  // When "swallow touches" is true, then returning 'true' from the
  // onTouchBegan method will "swallow" the touch event, preventing
  // other listeners from using it.
  t->setSwallowTouches(true);

  _eventDispatcher->addEventListenerWithSceneGraphPriority(t, this);
  CCLOG("init-touch: listener = %p", t);
}

//////////////////////////////////////////////////////////////////////////////
//
/*
void XGameLayer::update(float dt) {
  if (GetScene()->IsRunning() &&
      NNP(engine)) {
    engine->Update(dt);
  }
}
*/


NS_END(fusii)

