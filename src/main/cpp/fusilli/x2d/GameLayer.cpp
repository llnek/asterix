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
void GameLayer::postDeco() {
  if (NNP(engine)) {
    engine->forceSync();
    engine->ignite();
  }
  scheduleUpdate();
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::preDeco() {
  cx::resumeAudio();
  enableListeners();
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::surcease() {
  disableListeners();
  cx::pauseAudio();
  unscheduleUpdate();
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
  try { setKeyboardEnabled(false); } catch (...) { }
  try { setMouseEnabled(false); } catch (...) { }
  try { setTouchEnabled(false); } catch (...) { }
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
    if (!cx::isDesktop()) {
      setTouchMode(this->tmode);
      setTouchEnabled(true);
    }
  } catch (...) {
    CCLOG("failed to init-touch");
  }
  //CCLOG("enabled event handlers");
}

//////////////////////////////////////////////////////////////////////////////
//
bool GameLayer::onTouchesBegan(const s_vec<c::Touch*> &ts, c::Event*) {
  auto ok=true;
  F__LOOP(it,motionees) {
    auto c = *it;
    if (getTouchMode() == c::Touch::DispatchMode::ALL_AT_ONCE) {
      ok = ok && onTouchStart(c, ts);
    } else {
      ok = ok && onTouchStart(c, ts[0]);
    }
  }
  return motionees.size() > 0 ? ok : false;
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::onTouchesMoved(const s_vec<c::Touch*> &ts, c::Event*) {
  F__LOOP(it,motionees) {
    auto c = *it;
    if (getTouchMode() == c::Touch::DispatchMode::ALL_AT_ONCE) {
      onTouchMotion(c, ts);
    } else {
      onTouchMotion(c, ts[0]);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::onTouchesEnded(const s_vec<c::Touch*> &ts, c::Event*) {
  F__LOOP(it,motionees) {
    auto c = *it;
    if (getTouchMode() == c::Touch::DispatchMode::ALL_AT_ONCE) {
      onTouchEnd(c, ts);
    } else {
      onTouchEnd(c, ts[0]);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::onMouseMotion(ComObj *c, const c::Vec2 &loc) {
  c->setPos(loc.x, loc.y);
}

//////////////////////////////////////////////////////////////////////////////
//
bool GameLayer::onTouchStart(ComObj *c, const s_vec<c::Touch*> &ts) {
  throw "you need to implement this!";
}

//////////////////////////////////////////////////////////////////////////////
//
bool GameLayer::onTouchStart(ComObj *c, c::Touch *tap) {
  return true;
}


//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::onTouchEnd(ComObj *c, const s_vec<c::Touch*> &ts) {
  throw "you need to implement this!";
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::onTouchEnd(ComObj *c, c::Touch *tap) {
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::onTouchMotion(ComObj *c, const s_vec<c::Touch*> &ts) {
  throw "you need to implement this!";
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::onTouchMotion(ComObj *c, c::Touch *tap) {
  auto bx= MGMS()->getEnclosureBox();
  auto loc= tap->getLocation();
  auto pos= cx::clamp(tap, bx);
  c->setPos(pos.x, pos.y);
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
void GameLayer::onMouseMove(c::Event* event) {
  auto e= (c::EventMouse*)event;
  //auto loc= e->getLocationInView();
  auto b= e->getMouseButton();
  auto loc= e->getLocation();

  F__LOOP(it,motionees) {
    if (b == MOUSE_BUTTON_LEFT) {
      onMouseMotion(*it, loc);
    }
  }
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



NS_END



