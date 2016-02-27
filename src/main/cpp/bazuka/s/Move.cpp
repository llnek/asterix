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
#include "Move.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(bazuka)


//////////////////////////////////////////////////////////////////////////////
//
void Move::preamble() {
  players=engine->getNodeList(PlayerNode().typeId());
  shared=engine->getNodeList(SharedNode().typeId());
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
  auto ss= CC_GNLF(GVars, shared, "slots");
  auto py= CC_GNLF(Hero, players, "hero");

  switch(py->mPlayerState) {
    case kPlayerStateBoost: boostAnim(py); break;
    case kPLayerStateIdle: idleAnim(py); break;
  }

  if (!moveEnemies(dt)) { return;  }

  moveBullets(dt);
  moveRockets(dt);
  onKeys(dt);
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::moveBullets(float dt) {
  auto po= MGMS()->getPool("Bullets");
  auto wb= cx::visBox();
  auto &cs= po->list();

  F__LOOP(it,cs) {
    auto z = (Projectile*) *it;
    if (z->status) {
      z->sync();
      if (z->node->getPositionX() < wb.left) {
        z->deflate();
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::moveRockets(float dt) {
  auto po= MGMS()->getPool("Rockets");
  auto wb= cx::visBox();
  auto &cs= po->list();

  F__LOOP(it,cs) {
    auto z = (Projectile*) *it;
    if (z->status) {
      z->sync();
      if (z->node->getPositionX() > wb.right) {
        z->deflate();
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
bool Move::moveEnemies(float dt) {
  auto p= MGMS()->getPool("Enemies");
  auto wb= cx::visBox();
  auto over=false;
  auto &cs= p->list();

  F__LOOP(it, cs) {
    auto z= (Enemy*) *it;
    if (z->status) {
      z->sync();
      if (z->node->getPositionX() + HWZ(z->csize()) < wb.left) {
        over=true;
        break;
      }
    }
  }

  if (over) {
    SENDMSG("/game/player/loser");
  }

  return !over;
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::idleAnim(Hero *py) {
  if (py->mActionState != kActionStateIdle) {
    py->mActionState = kActionStateIdle;
    py->node->stopAllActions();
    py->node->runAction(mIdleAction);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::boostAnim(Hero *py) {
  if (py->mActionState != kActionStateBoost) {
    py->mActionState = kActionStateBoost;
    py->node->stopAllActions();
    py->node->runAction(mBoostAction);
  }
}


//////////////////////////////////////////////////////////////////////////////
//
void Move::onKeys(float dt) {

  auto mo= CC_GNLF(Gesture, players, "motion");
  auto py= CC_GNLF(Hero, players, "player");
  auto pos= py->pos();
  auto x=pos.x;
  auto y=pos.y;

  if (ENP(mo)) { return; }

  if (MGML()->keyPoll(KEYCODE::KEY_RIGHT_ARROW)) {
    x = pos.x + dt * py->speed;
  }
  if (MGML()->keyPoll(KEYCODE::KEY_LEFT_ARROW)) {
    x = pos.x - dt * py->speed;
  }
  if (MGML()->keyPoll(KEYCODE::KEY_UP_ARROW)) {
    y = pos.y + dt * py->speed;
  }
  if (MGML()->keyPoll(KEYCODE::KEY_DOWN_ARROW)) {
    y = pos.y - dt * py->speed;
  }

  py->setPos(x,y);


}


NS_END




