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
#include "AI.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(colorsmash)

//////////////////////////////////////////////////////////////////////////////
//
void AI::preamble() {
  _shared= _engine->getNodes("n/GVars")[0];
  _timer=cx::reifyTimer(MGML(), 1000);
  _time=60;
}

//////////////////////////////////////////////////////////////////////////////
//
bool AI::update(float dt) {
  if (MGMS()->isLive()) {
    //parallex(dt);
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::parallex(float dt) {
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::process(float dt) {
  if (!cx::timerDone(_timer)) { return; }  else {
    cx::undoTimer(_timer);
    _timer=CC_NIL;
  }
  --_time;
  auto msg= j::json({
      {"time", _time}
      });
  SENDMSGEX("/game/hud/updatetimer", &msg);

  if (_time <= 0) {
    SENDMSG("/game/player/lose");
    return;
  }
   if (_time == 5) {
    SENDMSG("/game/hud/redzone");
  }

  _timer=cx::reifyTimer(MGML(), 1000);
}


NS_END



