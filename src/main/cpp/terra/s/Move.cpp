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
#include "Move.h"
#include "Game.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////
//
void Move::preamble() {
  ship = engine->getNodeList(ShipNode().typeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Move::update(float dt) {
  if (MGMS()->isLive()) {
    moveMissiles(dt);
    move(dt);
    onKeys(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Move::onKeys(float dt) {
  auto ssp = CC_CSV(c::Float, "SHIP+SPEED");
  auto sp = CC_GNLF(Ship, ship, "ship");
  auto pos = sp->pos();
  auto wz= cx::visRect();
  auto x = pos.x;
  auto y = pos.y;
  auto ok = false;

  if (MGML()->keyPoll(KEYCODE::KEY_RIGHT_ARROW) &&
      pos.x <= wz.size.width) {
    x = pos.x + dt * ssp;
    ok= true;
  }
  if (MGML()->keyPoll(KEYCODE::KEY_LEFT_ARROW) &&
      pos.x >= 0) {
    x = pos.x - dt * ssp;
    ok= true;
  }
  if (MGML()->keyPoll(KEYCODE::KEY_DOWN_ARROW) &&
      pos.y >= 0) {
    y = pos.y - dt * ssp;
    ok= true;
  }
  if (MGML()->keyPoll(KEYCODE::KEY_UP_ARROW) &&
      pos.y <= wz.size.height) {
    y = pos.y + dt * ssp;
    ok= true;
  }

  if (ok) { sp->setPos(x,y); }
}

//////////////////////////////////////////////////////////////////////////
//
void Move::moveOneBomb(f::ComObj *bomb, float dt) {
  auto pos = bomb->pos();
  bomb->setPos(pos.x + bomb->vel.x * dt,
               pos.y + bomb->vel.y * dt);
}

//////////////////////////////////////////////////////////////////////////
//
void Move::move(float dt) {
  auto p = MGMS()->getPool("Bombs");
  p->foreach([=](f::ComObj* b) {
    if (b->status) {
      this->moveOneBomb(b,dt);
    }
  });
}

//////////////////////////////////////////////////////////////////////////
//
void Move::moveOneMissile(f::ComObj *m, float dt) {
  auto pos = m->pos();
  m->setPos(pos.x + m->vel.x * dt,
            pos.y + m->vel.y * dt);
}

//////////////////////////////////////////////////////////////////////////
//
void Move::moveMissiles(float dt) {
  auto p= MGMS()->getPool("Missiles");
  p->foreach([=](f::ComObj* v) {
    if (v->status) {
      this->moveOneMissile(v,dt);
    }
  });
}


NS_END(terra)


