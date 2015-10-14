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

#include "ash/Engine.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "XGameLayer.h"
#include "XScene.h"

NS_ALIAS(den,CocosDenshion)
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(fusii)


//////////////////////////////////////////////////////////////////////////////
//
XGameLayer::~XGameLayer() {
}

//////////////////////////////////////////////////////////////////////////////
//
XGameLayer::XGameLayer() {
  SNPTR(mouseListener)
  SNPTR(keysListener)
  SNPTR(touchListener)
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

//////////////////////////////////////////////////////////////////////////////
//
void XGameLayer::InizGame() {
  EnableEventHandlers();
  cx::PauseAudio();
  OnNewGame();
  this->scheduleUpdate();
}

//////////////////////////////////////////////////////////////////////////////
//
void XGameLayer::FinzGame() {
  DisableEventHandlers();
  cx::PauseAudio();
  OnGameOver();
  this->unscheduleUpdate();
}

//////////////////////////////////////////////////////////////////////////////
//
void XGameLayer::DisableEventHandlers() {
  CCLOG("disabling event handlers");
  if (NNP(mouseListener)) {
    _eventDispatcher->removeEventListener(mouseListener);
  }
  if (NNP(keysListener)) {
    _eventDispatcher->removeEventListener(keysListener);
  }
  if (NNP(touchListener)) {
    _eventDispatcher->removeEventListener(touchListener);
  }
  for (int n=0; n < 256; ++n) {
    keyboard[n]=false;
  }

  SNPTR(mouseListener)
  SNPTR(keysListener)
  SNPTR(touchListener)
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
  auto ln = c::EventListenerMouse::create();

  ln->onMouseMove = CC_CALLBACK_1(XGameLayer::OnMouseMove, this);
  ln->onMouseUp = CC_CALLBACK_1(XGameLayer::OnMouseUp, this);
  ln->onMouseDown = CC_CALLBACK_1(XGameLayer::OnMouseDown, this);
  ln->onMouseScroll = CC_CALLBACK_1(XGameLayer::OnMouseScroll, this);

  _eventDispatcher->addEventListenerWithSceneGraphPriority(ln, this);

  CCLOG("init-mouse: listener = %p", ln);
  mouseListener=ln;
}

//////////////////////////////////////////////////////////////////////////////
//
void XGameLayer::OnMouseMove(c::Event* e) {
  auto evt = (c::EventMouse*) e;
  evt->getDelta();
  evt->getLocation();
  //CCLOG("mouse moved");
}

//////////////////////////////////////////////////////////////////////////////
//
void XGameLayer::OnMouseDown(c::Event*) {
  //CCLOG("mouse down");
}


//////////////////////////////////////////////////////////////////////////////
//
void XGameLayer::OnMouseUp(c::Event*) {
 // CCLOG("mouse up");
}

//////////////////////////////////////////////////////////////////////////////
//
void XGameLayer::OnMouseScroll(c::Event*) {
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

  auto ln = c::EventListenerKeyboard::create();

  ln->onKeyReleased = CC_CALLBACK_2(XGameLayer::OnKeyReleased, this);
  ln->onKeyPressed = CC_CALLBACK_2(XGameLayer::OnKeyPressed, this);

  _eventDispatcher->addEventListenerWithSceneGraphPriority(ln, this);

  CCLOG("init-keys: listener = %p", ln);
  keysListener=ln;
}

//////////////////////////////////////////////////////////////////////////////
//
void XGameLayer::InitTouch() {
  //  Reify a "one by one" touch event listener
  // (processes one touch at a time)
  auto ln= c::EventListenerTouchOneByOne::create();

  // trigger when you push down
  ln->onTouchBegan = [](c::Touch* touch, c::Event* event){

      return true; // if you are consuming it
  };

  // trigger when moving touch
  ln->onTouchMoved = [](c::Touch* touch, c::Event* event){
      // your code
  };

  // trigger when you let up
  ln->onTouchEnded = [=](c::Touch* touch, c::Event* event){
      // your code
  };

  // When "swallow touches" is true, then returning 'true' from the
  // onTouchBegan method will "swallow" the touch event, preventing
  // other listeners from using it.
  ln->setSwallowTouches(true);

  _eventDispatcher->addEventListenerWithSceneGraphPriority(ln, this);


  CCLOG("init-touch: listener = %p", ln);
  touchListener=ln;
}

//////////////////////////////////////////////////////////////////////////////
//
XLayer* XGameLayer::GetBackgd(int tag) {
  return GetScene()->GetLayer(tag);
}

//////////////////////////////////////////////////////////////////////////////
//
XHUDLayer* XGameLayer::GetHUD(int tag) {
  return SCAST(XHUDLayer*, GetScene()->GetLayer(tag));
}

//////////////////////////////////////////////////////////////////////////////
//
void XGameLayer::update(float dt) {
  if (GetScene()->IsRunning() &&
      NNP(engine)) {
    engine->Update(dt);
  }
}



NS_END(fusii)

