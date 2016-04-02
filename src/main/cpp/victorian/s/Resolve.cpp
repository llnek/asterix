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

//////////////////////////////////////////////////////////////////////////////

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "n/Terrain.h"
#include "n/Player.h"
#include "Resolve.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(victorian)


//////////////////////////////////////////////////////////////////////////////
//
void Resolve::preamble() {
  _terrain= _engine->getNodes("f/CAutoma")[0];
  _player= _engine->getNodes("f/CGesture")[0];
  _shared= _engine->getNodes("n/GVars")[0];
}

//////////////////////////////////////////////////////////////////////////////
//
bool Resolve::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::process(float dt) {
  auto pm=CC_GEC(PlayerMotion,_player,"f/CMove");
  auto py=CC_GEC(Player,_player,"f/CPixie");
  auto tn=CC_GEC(Terrain,_terrain,"f/CPixie");
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto wb=cx::visBox();

  if (py->getPositionY() < - cx::getHeight(py) ||
      py->getPositionX() < - HTV(cx::getWidth(py)) ) {
    if (ss->state == kGamePlay) {
      //_running = false;

        //create GAME OVER state

      ss->state = kGameOver;

      //_tryAgain->setVisible(true);
      //
      //_scoreDisplay->setAnchorPoint(Vec2(0.5f, 0.5f));
      //_scoreDisplay->setPosition(Vec2(_screenSize.width * 0.5f,
                                     //_screenSize.height * 0.88f));
      ss->hat->setPosition(wb.right * 0.2, -wb.top * 0.1);
      CC_SHOW(ss->hat);

      auto rotate = c::RotateBy::create(2.0, 660);
      auto jump = c::JumpBy::create(2.0, CCT_PT(0,10), wb.top * 0.8, 1);
      ss->hat->runAction(rotate);
      ss->hat->runAction(jump);
      cx::pauseMusic();
      cx::sfxPlay("crashing");
      SENDMSG("/game/player/lose");
    }
  }

  //update score
  if (tn->isStartTerrain() && pm->vel.x > 0) {
    int v= dt * 50;
    auto msg= j::json({
        {"score", v}
    });

    SENDMSGEX("/game/player/earnscore", &msg);

    ss->speedIncreaseTimer += dt;
    if (ss->speedIncreaseTimer > ss->speedIncreaseInterval) {
      ss->speedIncreaseTimer = 0;
      pm->maxSpeed.x = pm->maxSpeed.x +4;
    }
  }

}




NS_END




