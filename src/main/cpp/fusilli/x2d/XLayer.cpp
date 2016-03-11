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
  return (XScene*) getParent();
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
    if (!cx::isDesktop()) {
      //setTouchMode(this->tmode);
      setTouchEnabled(true);
    }
  } catch (...) {
    CCLOG("failed to init-touch");
  }
  //CCLOG("enabled event handlers");
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::onTouchesBegan(const s_vec<c::Touch*> &ts, c::Event*) {
  if (_tMode == c::Touch::DispatchMode::ALL_AT_ONCE) {
    onTouchStart( ts);
  } else {
    onTouchStart( ts[0]);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::onTouchesMoved(const s_vec<c::Touch*> &ts, c::Event*) {
  if (_tMode == c::Touch::DispatchMode::ALL_AT_ONCE) {
    onTouchMotion( ts);
  } else {
    onTouchMotion( ts[0]);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::onTouchesEnded(const s_vec<c::Touch*> &ts, c::Event*) {
  if (_tMode == c::Touch::DispatchMode::ALL_AT_ONCE) {
    onTouchEnd( ts);
  } else {
    onTouchEnd( ts[0]);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::onMouseMotion( const c::Vec2 &loc) {
}

//////////////////////////////////////////////////////////////////////////////
//
bool XLayer::onTouchStart( const s_vec<c::Touch*> &ts) {
  throw "you need to implement this!";
}

//////////////////////////////////////////////////////////////////////////////
//
bool XLayer::onTouchStart( c::Touch *tap) {
  return true;
}


//////////////////////////////////////////////////////////////////////////////
//
void XLayer::onTouchEnd( const s_vec<c::Touch*> &ts) {
  throw "you need to implement this!";
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::onTouchEnd( c::Touch *tap) {
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::onTouchMotion( const s_vec<c::Touch*> &ts) {
  throw "you need to implement this!";
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::onTouchMotion( c::Touch *tap) {
  //auto bx= MGMS()->getEnclosureBox();
  //auto loc= tap->getLocation();
  //auto pos= cx::clamp(loc, bx);
  //c->setPos(pos.x, pos.y);
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::onKeyPressed(KEYCODE k, c::Event*) {
  int n= (int)k;
  if (n >= 0 && n < 256) {
    this->_keys[n]= true;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::onKeyReleased(KEYCODE k, c::Event*) {
  int n= (int)k;
  if (n >= 0 && n < 256) {
    this->_keys[n]=false;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::onMouseDown(c::Event*) {
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::onMouseUp(c::Event* event) {

  auto e= (c::EventMouse*)event;
  auto loc= e->getLocationInView();
  auto b= e->getMouseButton();

  if (b == MOUSE_BUTTON_LEFT) {
    onMouseClick( loc);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::onMouseClick( const c::Vec2 &loc) {

}

//////////////////////////////////////////////////////////////////////////////
//
void XLayer::onMouseMove(c::Event* event) {
  auto e= (c::EventMouse*)event;
  auto loc= e->getLocationInView();
  auto b= e->getMouseButton();

  if (b == MOUSE_BUTTON_LEFT) {
    onMouseMotion( loc);
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
      if (NNP(_mouseListener)) { return; }
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
      SNPTR(_mouseListener)
    }
  }
}


NS_END


