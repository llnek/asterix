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

#include "x2d/GameScene.h"
#include "core/CCSX.h"
#include "Render.h"
#include "Game.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////
//
void Render::preamble() {
  _arena=_engine->getNodes("n/GVars")[0];
}

//////////////////////////////////////////////////////////////////////////
//
bool Render::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Render::process(float dt) {
  // background's moving rate is 16 pixel per second
  auto ss= CC_GEC(GVars,_arena,"n/GVars");
  auto locSkyHeight = ss->backSkyDim.height;
  auto locBackSkyRe = ss->backSkyRe;
  auto locBackSky = ss->backSky;
  auto ui= CC_GEC(f::CPixie,locBackSky,"f/CPixie");
  auto posy= ui->node->getPositionY();
  auto movingDist = 16.0 * dt;
  auto wz = cx::visRect();
  auto currPosY = posy - movingDist;

  if (locSkyHeight + currPosY <= wz.size.height) {

    if (NNP(locBackSkyRe)) {
      throw "The memory is leaking at moving background";
    }

    ss->backSkyRe = ss->backSky;
    locBackSkyRe = ss->backSky;

    //create a new background
      ss->backSky = (ecs::Node*)MGMS()->getPool("BackSkies")->take();
    locBackSky = ss->backSky;
      ui=CC_GEC(f::CPixie,locBackSky,"f/CPixie");
    ui->inflate(0, currPosY + locSkyHeight - 2);
  } else {
    ui->node->setPositionY(currPosY);
  }

  if (locBackSkyRe) {
    ui=CC_GEC(f::CPixie,locBackSkyRe,"f/CPixie");
    currPosY = ui->node->getPositionY() - movingDist;
    if (currPosY + locSkyHeight < 0.0) {
      ss->backSkyRe = CC_NIL;
      ui->deflate();
      locBackSkyRe->yield();
    } else {
      ui->node->setPositionY(currPosY);
    }
  }
}


NS_END


