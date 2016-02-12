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
  terrains=engine->getNodeList(TerrainNode().typeId());
  players=engine->getNodeList(PlayerNode().typeId());
  shared=engine->getNodeList(SharedNode().typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::process(float dt) {
  auto tn=CC_GNLF(Terrain,terrains,"terrain");
  auto py=CC_GNLF(Player,players,"player");
  auto ss=CC_GNLF(GVars,shared,"slots");
  auto atlas=MGML()->getAtlas("game-pics");
    auto wb=cx::visBox();
  py->update(dt);
  tn->move(py->vel.x);

  if (py->getState() != kPlayerDying) {
    tn->checkCollision(py);
  }

  py->place();

  if (py->nextPos.y > wb.top * 0.6f) {
    atlas->setPositionY((wb.top * 0.6f - py->nextPos.y) * 0.8f);
  } else {
    atlas->setPositionY(0);
  }

    //update paralax
  if (py->vel.x > 0) {
    ss->background->setPositionX(
        ss->background->getPosition().x - py->vel.x * 0.25f);

    float diffx;
    if (ss->background->getPositionX() < -ss->background->getContentSize().width) {
      diffx = fabs(ss->background->getPositionX()) - ss->background->getContentSize().width;
      ss->background->setPositionX(-diffx);
    }

    ss->foreground->setPositionX(
        ss->foreground->getPosition().x - py->vel.x * 4);

    if (ss->foreground->getPositionX() < -ss->foreground->getContentSize().width * 4) {
      diffx = fabs(ss->foreground->getPositionX()) - ss->foreground->getContentSize().width * 4;
      ss->foreground->setPositionX(-diffx);
    }

    auto &ps=MGMS()->getPool("Clouds")->list();
    F__LOOP(it,ps) {
      auto c= *it;
      c->sprite->setPositionX(
          c->sprite->getPositionX() - py->vel.x * 0.15f);
      if (c->sprite->getPositionX() +
          c->sprite->boundingBox().size.width * 0.5f < 0 )
        c->sprite->setPositionX(
            wb.right + c->sprite->boundingBox().size.width * 0.5f);
    }
  }

  if (ss->jam->isVisible()) {
    if (ss->jam->getPositionX() < -wb.right * 0.2f) {
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




