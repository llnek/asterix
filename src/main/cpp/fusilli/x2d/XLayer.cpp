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

#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventListenerMouse.h"
#include "base/CCEventListenerTouch.h"
#include "renderer/CCTextureCache.h"
#include "2d/CCMenuItem.h"
#include "2d/CCSprite.h"
#include "2d/CCMenu.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "XScene.h"
#include "XLayer.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
bool XLayer::initEx(XScene *par, int zx) {
  if (c::Layer::init()) {
    par->addLayer(this, zx);
    decoPre();
    decoUI();
    decoPost();
    return true;
  } else {
    return false;
  }
}

//////////////////////////////////////////////////////////////////////////////
// Remember the parent scene object
//
XScene* XLayer::getSceneX() {
  return static_cast<XScene*>( getParent());
}

//////////////////////////////////////////////////////////////////////////////
//
XLayer::XLayer() {
  bind(this);
}

//////////////////////////////////////////////////////////////////////////////
//
bool XLayer::keyPoll(KEYCODE key) {
  int k= (int) key;
  return k >= 0 && k < 256 ? _keys[k] : false;
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::disableListeners() {
  //CCLOG("disabling event handlers");
  try { setKeyboardEnabled(false); } catch (...) { }
  try { setMouseEnabled(false); } catch (...) { }
  try { setTouchEnabled(false); } catch (...) { }
  _keys.fill(false);
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::enableListeners() {
  disableListeners();
  try {
    if (cx::isDesktop()) setKeyboardEnabled(true);
  } catch (...) {
    CCLOG("failed to init-keys");
  }
  try {
    if (cx::isDesktop()) setMouseEnabled(true);
  } catch (...) {
    CCLOG("failed to init-mouse");
  }
  try {
    if (!cx::isDesktop()) setTouchEnabled(true);
  } catch (...) {
    CCLOG("failed to init-touch");
  }
  //CCLOG("enabled event handlers");
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::onTouchesBegan(const VecTouches &ts, c::Event*) {
  if (_tMode == c::Touch::DispatchMode::ALL_AT_ONCE) {
    onTouchStart(ts);
  } else {
    onTouchStart(ts[0]);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::onTouchesMoved(const VecTouches &ts, c::Event*) {
  if (_tMode == c::Touch::DispatchMode::ALL_AT_ONCE) {
    onTouchMotion(ts);
  } else {
    onTouchMotion(ts[0]);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::onTouchesEnded(const VecTouches &ts, c::Event*) {
  if (_tMode == c::Touch::DispatchMode::ALL_AT_ONCE) {
    onTouchEnd(ts);
  } else {
    onTouchEnd(ts[0]);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::onMouseMotion(const CCT_PT &loc) {
}

//////////////////////////////////////////////////////////////////////////////
//
bool XLayer::onTouchStart(const VecTouches &ts) {
  throw "you need to implement this!";
}

//////////////////////////////////////////////////////////////////////////////
//
bool XLayer::onTouchStart(c::Touch *tap) {
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::onTouchEnd(const VecTouches &ts) {
  throw "you need to implement this!";
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::onTouchEnd(c::Touch *tap) {
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::onTouchMotion(const VecTouches &ts) {
  throw "you need to implement this!";
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::onTouchMotion(c::Touch *tap) {
  //auto bx= MGMS()->getEnclosureBox();
  //auto loc= tap->getLocation();
  //auto pos= cx::clamp(loc, bx);
  //c->setPos(pos.x, pos.y);
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::onKeyPressed(KEYCODE k, c::Event*) {
  int n= (int)k;
  if (n >= 0 && n < _keys.size()) {
    this->_keys[n]= true;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::onKeyReleased(KEYCODE k, c::Event*) {
  int n= (int)k;
  if (n >= 0 && n < _keys.size()) {
    this->_keys[n]=false;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::onMouseDown(c::Event *event) {
  if (_mouseDown) { return; }
  auto e= (c::EventMouse*)event;
  _mouseTarget=false;
  _mouseDown=true;
  if (e->getMouseButton() == _mouseBtn) {
    _mouseTarget=onMouseStart(e->getLocationInView());
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::onMouseUp(c::Event *event) {
  auto e= (c::EventMouse*)event;
  if (e->getMouseButton() == _mouseBtn) {
    onMouseClick(e->getLocationInView());
  }
  _mouseTarget=false;
  _mouseDown=false;
}

//////////////////////////////////////////////////////////////////////////////
//
bool XLayer::onMouseStart(const CCT_PT &loc) {
  return true;
  //CCLOG("mouse Down!");
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::onMouseClick(const CCT_PT &loc) {
  //CCLOG("mouse Up!");
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::onMouseMove(c::Event *event) {
  auto e= (c::EventMouse*)event;
  if (_mouseTarget &&
      _mouseBtn == e->getMouseButton()) {
    onMouseMotion(e->getLocationInView());
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::onMouseScroll(c::Event*) {
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::setMouseEnabled(bool enabled) {
  if (_mouseEnabled != enabled) {
    _mouseEnabled = enabled;
    if (enabled) {
      if (N_NIL(_mouseListener)) { return; }
      auto n = c::EventListenerMouse::create();
      _mouseListener = n;
      n->onMouseScroll = CC_CALLBACK_1(XLayer::onMouseScroll, this);
      n->onMouseMove = CC_CALLBACK_1(XLayer::onMouseMove, this);
      n->onMouseUp = CC_CALLBACK_1(XLayer::onMouseUp, this);
      n->onMouseDown = CC_CALLBACK_1(XLayer::onMouseDown, this);
      _eventDispatcher->addEventListenerWithSceneGraphPriority(n, this);
    }
    else {
      _eventDispatcher->removeEventListener(_mouseListener);
      S__NIL(_mouseListener)
    }
  }
}


NS_END


