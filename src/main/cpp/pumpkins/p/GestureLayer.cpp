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
#include "GestureLayer.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(pumpkins)

//////////////////////////////////////////////////////////////////////////////
//
owner<GestureLayer*> GestureLayer::create(
    c::Node *target, SEL_CallFuncO handler) {

  auto z = mc_new(GestureLayer);
  z->inix(target, handler);
  z->autorelease();
  return z;
}

//////////////////////////////////////////////////////////////////////////////
//
bool GestureLayer::inix(c::Node *target, SEL_CallFuncO handler) {

  if (target == CC_NIL || handler == CC_NIL) {
    CCLOGERROR("GestureLayer received invalid arguments");
    return false;
  }

  if (! c::Layer::init()) {
  return false; }

  _handler = handler;
  _target = target;
  setTouchEnabled(true);
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void GestureLayer::onTouchStart(c::Touch *touch) {
  onMouseStart(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void bool GestureLayer::onMouseStart(const CCT_PT &tap) {
  _gestureType = E_GESTURE_NONE;
  _touchStart = tap;
  _touchEnd = CC_ZPT;
  _touchActive = true;
}

//////////////////////////////////////////////////////////////////////////////
//
void GestureLayer::onTouchMotion(c::Touch *touch) {
  onMouseMotion(touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GestureLayer::onMouseMotion(const CCT_PT &tap) {
  _touchEnd = tap;
  handleTouch();
}

//////////////////////////////////////////////////////////////////////////////
//
void GestureLayer::onTouchEnd(c::Touch *touch) {
  onMouseClick( touch->getLocation());
}

//////////////////////////////////////////////////////////////////////////////
//
void GestureLayer::onMouseClick(const CCT_PT &tap) {
  _touchEnd = tap;
  handleTouch();
  _touchActive = false;
}

//////////////////////////////////////////////////////////////////////////////
//
void GestureLayer::handleTouch() {

  if (!_touchActive) {
  return; }

  // check for a single tap
  if (c::ccpFuzzyEqual(_touchStart, _touchEnd, 1)) {
    _gestureType = E_GESTURE_TAP;
    (_target->*_handler)(this);
    _touchActive = false;
    return;
  }

  // calculate distance between first and last touch
  auto touch_difference = c::ccpSub(_touchEnd, _touchStart);
  // horizontal swipe
  if (fabs(touch_difference.x) > MIN_GESTURE_DISTANCE) {
    _gestureType = (touch_difference.x > 0) ? E_GESTURE_SWIPE_RIGHT : E_GESTURE_SWIPE_LEFT;
    (_target->*_handler)(this);
    _touchActive = false;
    return;
  }

  // vertical swipe
  if (fabs(touch_difference.y) > MIN_GESTURE_DISTANCE) {
    _gestureType = (touch_difference.y > 0) ? E_GESTURE_SWIPE_UP : E_GESTURE_SWIPE_DOWN;
    (_target->*_handler)(this);
    _touchActive = false;
    return;
  }

}



NS_END


