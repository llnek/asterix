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

#include "x2d/GameScene.h"
#include "core/CCSX.h"
#include "Render.h"
#include "Game.h"
#include "ash/Node.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
//
Render::Render(not_null<EFactory*> e, not_null<c::Dictionary*> d)

  : BaseSystem<EFactory>(e, d) {

}

//////////////////////////////////////////////////////////////////////////
//
void Render::addToEngine(not_null<a::Engine*> e) {

}

//////////////////////////////////////////////////////////////////////////
//
bool Render::update(float dt) {
  if (MGMS()->isLive()) {
    processMovement(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Render::processMovement(float dt) {
  // background's moving rate is 16 pixel per second
  auto g= (Game*) MGMS();
  auto locSkyHeight = g->backSkyDim.height;
  auto locBackSkyRe = g->backSkyRe;
  auto locBackSky = g->backSky;
  auto posy= locBackSky->sprite->getPositionY();
  auto movingDist = 16.0f * dt;
  auto wz = cx::visRect();
  auto currPosY = posy - movingDist;

  if (locSkyHeight + currPosY <= wz.size.height) {

    if (NNP(locBackSkyRe)) {
      throw "The memory is leaking at moving background";
    }

    g->backSkyRe = g->backSky;
    locBackSkyRe = g->backSky;

    //create a new background
    g->backSky = MGMS()->getPool("BackSkies")->get();
    locBackSky = g->backSky;
    locBackSky->inflate(0, currPosY + locSkyHeight - 2);
  } else {
    locBackSky->sprite->setPositionY(currPosY);
  }

  if (NNP(locBackSkyRe)) {
    currPosY = locBackSkyRe->sprite->getPositionY() - movingDist;
    if (currPosY + locSkyHeight < 0.0f) {
      g->backSkyRe = nullptr;
      locBackSkyRe->deflate();
    } else {
      locBackSkyRe->sprite->setPositionY(currPosY);
    }
  }
}




NS_END(terra)


