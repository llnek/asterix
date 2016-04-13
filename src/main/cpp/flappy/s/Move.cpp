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

//////////////////////////////////////////////////////////////////////////////

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Move.h"
#include "n/Dragon.h"
#include "n/Tower.h"
#include "n/Fairytale.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(flappy)


//////////////////////////////////////////////////////////////////////////////
//
void Move::preamble() {
  _shared= _engine->getNodes("n/GVars")[0];
}

//////////////////////////////////////////////////////////////////////////////
//
bool Move::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::process(float dt) {
  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto wb= cx::visBox();

  ss->dragon->update(dt);

  // update towers only after game has started
  if (ss->hasGameStarted) {
      ss->towers->update(dt);
  }

  // update environment
  ss->fairytale->update(dt);

  onKeys(dt);
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::onKeys(float dt) {
  if (MGML()->keyPoll(KEYCODE::KEY_RIGHT_ARROW)) {
  }
  if (MGML()->keyPoll(KEYCODE::KEY_LEFT_ARROW)) {
  }
  if (MGML()->keyPoll(KEYCODE::KEY_UP_ARROW)) {
  }
  if (MGML()->keyPoll(KEYCODE::KEY_DOWN_ARROW)) {
  }
}


NS_END




