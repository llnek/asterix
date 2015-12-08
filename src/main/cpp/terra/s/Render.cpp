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

#include "Render.h"
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
//
Render::Render(not_null<a::Engine*> e, not_null<c::Dictionary*> d)

  : BaseSystem<EFactory>(e, d) {

}

//////////////////////////////////////////////////////////////////////////
//
void Render::addToEngine(not_null<a::Engine*> e) {

}

//////////////////////////////////////////////////////////////////////////
//
bool Render::onUpdate(float dt) {
  if (MGMS()->isLive()) {
    processMovement(dt);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Render::processMovement(float dt) {
  // background's moving rate is 16 pixel per second
  auto locSkyHeight = MGMS()->backSkyDim.height;
  auto locBackSkyRe = MGMS()->backSkyRe;
  auto locBackSky = MGMS()->backSky;
  auto posy= locBackSky->sprite->getPositionY();
  auto movingDist = 16.0f * dt;
  auto wz = cx::visRect();
  auto currPosY = posy - movingDist;

  if (locSkyHeight + currPosY <= wz.height) {

    if (NNP(locBackSkyRe)) {
      throw "The memory is leaking at moving background";
    }

    MGMS()->backSkyRe = MGMS()->backSky;
    locBackSkyRe = MGMS()->backSky;

    //create a new background
    MGMS()->backSky = MGMS()->getPool("BackSkies")->get();
    locBackSky = MGMS()->backSky;
    locBackSky->inflate(0, currPosY + locSkyHeight - 2);
  } else {
    locBackSky->sprite->setPositionY(currPosY);
  }

  if (NNP(locBackSkyRe)) {
    currPosY = locBackSkyRe->sprite->getPositionY() - movingDist;
    if (currPosY + locSkyHeight < 0.0f) {
      MGMS()->backSkyRe = nullptr;
      locBackSkyRe->deflate();
    } else {
      locBackSkyRe->sprite->setPositionY(currPosY);
    }
  }
}




NS_END(terra)


