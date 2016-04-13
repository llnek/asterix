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
NS_BEGIN(victorian)

//////////////////////////////////////////////////////////////////////////////
//
void AI::preamble() {
  _player=_engine->getNodes("f/CGesture")[0];
  _shared=_engine->getNodes("n/GVars")[0];
}

//////////////////////////////////////////////////////////////////////////////
//
bool AI::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void AI::process(float dt) {

  auto pm=CC_GEC(PlayerMotion, _player,"f/CMove");
  auto ps=CC_GEC(PlayerStats, _player,"f/CStats");
  auto py=CC_GEC(Player, _player,"f/CPixie");
  auto ss=CC_GEC(GVars, _shared,"n/GVars");

    //update parallax
  if (pm->vel.x > 0) {
    ss->background->setPositionX(
        ss->background->getPositionX() - pm->vel.x * 0.25);

    if (ss->background->getPositionX() < - CC_CSIZE(ss->background).width) {
      auto diffx = fabs(ss->background->getPositionX()) - CC_CSIZE(ss->background).width;
      ss->background->setPositionX(-diffx);
    }

    ss->foreground->setPositionX(
        ss->foreground->getPositionX() - pm->vel.x * 4);

    if (ss->foreground->getPositionX() < - CC_CSIZE(ss->foreground).width * 4) {
      auto diffx = fabs(ss->foreground->getPositionX()) - CC_CSIZE(ss->foreground).width * 4;
      ss->foreground->setPositionX(-diffx);
    }

    auto ps=MGMS()->getPool("Clouds")->ls();
      auto wb=cx::visBox();
    F__LOOP(it,ps) {
      auto e= *it;
      auto ui=CC_GEC(f::CPixie,e,"f/CPixie");
      auto sz= CC_CSIZE(ui);
      ui->setPositionX(
          ui->getPositionX() - pm->vel.x * 0.15);
      if (ui->getPositionX() + HWZ(sz) < 0 )
        ui->setPositionX( wb.right + HWZ(sz));
    }
  }
}



NS_END



