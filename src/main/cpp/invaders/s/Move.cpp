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
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Move.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////
//
void Move::preamble() {
  ship = engine->getNodeList(ShipMotionNode().typeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Move::update(float dt) {
  if (MGMS()->isLive()) {
    processShipMotions( dt);
    moveMissiles(dt);
    moveBombs(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Move::processShipMotions( float dt) {

  auto sp= CC_GNLF(Ship, ship,"ship");
  auto pos = sp->pos();
  auto x= pos.x;
  auto y= pos.y;

  if (MGML()->keyPoll(KEYCODE::KEY_RIGHT_ARROW)) {
    x = pos.x + dt * sp->vel.x;
  }

  if (MGML()->keyPoll(KEYCODE::KEY_LEFT_ARROW)) {
    x = pos.x - dt * sp->vel.x;
  }

  sp->setPos(x,y);
  clamp(sp);
}

//////////////////////////////////////////////////////////////////////////
//
void Move::clamp(Ship *ship) {

  auto sz= ship->sprite->getContentSize();
  auto tile= CC_CSV(c::Float,"TILE");
  auto wz = cx::visRect();
  auto pos= ship->pos();

  if (cx::getRight(ship->sprite) > wz.size.width - tile) {
    ship->setPos(wz.size.width - tile - sz.width * 0.5f, pos.y);
  }
  if (cx::getLeft(ship->sprite) < tile) {
    ship->setPos(tile + sz.width * 0.5f, pos.y);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Move::moveBombs(float dt) {

  auto bbs= MGMS()->getPool("bombs");
  auto c= bbs->list();

  F__LOOP(it, c) {
    auto &b = *it;
    if (b->status) {
      auto pos= b->pos();
      auto y = pos.y + dt * b->vel.y;
      b->setPos(pos.x, y);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Move::moveMissiles(float dt) {

  auto mss= MGMS()->getPool("missiles");
  auto c= mss->list();

  F__LOOP(it, c) {

    auto &m = *it;
    auto pos= m->pos();
    auto y = pos.y + dt * m->vel.y;
    m->setPos(pos.x, y);
  }
}



NS_END(invaders)



