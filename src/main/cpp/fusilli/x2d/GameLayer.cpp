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
#include "GameScene.h"
#include "GameLayer.h"

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
void GameLayer::decoPost() {
  if (NNP(engine)) {
    engine->ignite();
  }
  scheduleUpdate();
}

//////////////////////////////////////////////////////////////////////////////
//
void GameLayer::decoPre() {
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



