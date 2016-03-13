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
#include "Move.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(victorian)

//////////////////////////////////////////////////////////////////////////////
//
void Move::preamble() {
  _terrain= _engine->getNodes("n/Terrain")[0];
  _player= _engine->getNodes("f/CGesture")[0];
  _shared= _engine->getNodes("n/GVars")[0];
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::process(float dt) {
  auto tn=CC_GEC(Terrain, _terrain,"n/Terrain");
  auto py=CC_GEC(Player, _player,"f/CDraw");
  auto ss=CC_GEC(GVars, _shared,"n/GVars");
  auto atlas=MGML()->getAtlas("game-pics");
  auto wb=cx::visBox();

  py->update(dt);
  tn->move(py->vel.x);

  if (py->getState() != kPlayerDying) {
    tn->checkCollision(py);
  }

  py->place();

  if (py->nextPos.y > wb.top * 0.6) {
    atlas->setPositionY((wb.top * 0.6 - py->nextPos.y) * 0.8);
  } else {
    atlas->setPositionY(0);
  }

    //update parallax
  if (py->vel.x > 0) {
    ss->background->setPositionX(
        ss->background->getPositionX() - py->vel.x * 0.25);

    float diffx;
    if (ss->background->getPositionX() < - CC_CSIZE(ss->background).width) {
      diffx = fabs(ss->background->getPositionX()) - CC_CSIZE(ss->background).width;
      ss->background->setPositionX(-diffx);
    }

    ss->foreground->setPositionX(
        ss->foreground->getPositionX() - py->vel.x * 4);

    if (ss->foreground->getPositionX() < - CC_CSIZE(ss->foreground).width * 4) {
      diffx = fabs(ss->foreground->getPositionX()) - CC_CSIZE(ss->foreground).width * 4;
      ss->foreground->setPositionX(-diffx);
    }

    auto ps=MGMS()->getPool("Clouds")->ls();
    F__LOOP(it,ps) {
      auto e= (ecs::Node*) *it;
      auto ui=CC_GEC(f::CDraw,e,"f/CDraw");
      ui->node->setPositionX(
          ui->node->getPositionX() - py->vel.x * 0.15);
      if (ui->node->getPositionX() + HWZ(ui->csize()) < 0 )
        ui->node->setPositionX(
            wb.right + HWZ(ui->csize()));
    }
  }

  if (ss->jam->isVisible()) {
    if (ss->jam->getPositionX() < -wb.right * 0.2) {
      ss->jam->stopAllActions();
      CC_HIDE(ss->jam);
    }
  }

}

//////////////////////////////////////////////////////////////////////////////
//
bool Move::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}


NS_END




