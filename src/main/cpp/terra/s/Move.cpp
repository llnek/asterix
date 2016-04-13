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

#include "x2d/GameScene.h"
#include "core/CCSX.h"
#include "Move.h"
#include "Game.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////
//
void Move::preamble() {
  _ship = _engine->getNodes("f/CGesture")[0];
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
  auto sp = CC_GEC(Ship, _ship, "f/CPixie");
  auto p = CC_CSV(c::Float, "SHIP+SPEED");
  auto pos = sp->getPosition();
  auto wb= cx::visBox();
  auto x = pos.x;
  auto y = pos.y;
  auto ok = false;

  p= p * dt;

  if (MGML()->keyPoll(KEYCODE::KEY_RIGHT_ARROW) &&
      pos.x <= wb.right) {
    x = pos.x + p;
    ok= true;
  }
  if (MGML()->keyPoll(KEYCODE::KEY_LEFT_ARROW) &&
      pos.x >= wb.left) {
    x = pos.x - p;
    ok= true;
  }
  if (MGML()->keyPoll(KEYCODE::KEY_DOWN_ARROW) &&
      pos.y >= wb.bottom) {
    y = pos.y - p;
    ok= true;
  }
  if (MGML()->keyPoll(KEYCODE::KEY_UP_ARROW) &&
      pos.y <= wb.top) {
    y = pos.y + p;
    ok= true;
  }

  if (ok) { CC_POS2(sp,x,y); }
}

//////////////////////////////////////////////////////////////////////////
//
void Move::moveOneBomb(ecs::Node *bomb, float dt) {
  auto mv=CC_GEC(f::CMove,bomb,"f/CMove");
  auto ui=CC_GEC(Missile,bomb,"f/CPixie");
  auto pos = ui->getPosition();

  CC_POS2(ui,pos.x + mv->vel.x * dt,
             pos.y + mv->vel.y * dt);
}

//////////////////////////////////////////////////////////////////////////
//
void Move::move(float dt) {
  auto po = MGMS()->getPool("Bombs");
  po->foreach([=](f::Poolable* p) {
    if (p->status()) {
      this->moveOneBomb((ecs::Node*)p,dt);
    }
  });
}

//////////////////////////////////////////////////////////////////////////
//
void Move::moveOneMissile(ecs::Node *m, float dt) {
  auto ui=CC_GEC(Missile,m,"f/CPixie");
  auto mv=CC_GEC(f::CMove,m,"f/CMove");
  auto pos = ui->getPosition();
  CC_POS2(ui,pos.x + mv->vel.x * dt,
             pos.y + mv->vel.y * dt);
}

//////////////////////////////////////////////////////////////////////////
//
void Move::moveMissiles(float dt) {
  auto po= MGMS()->getPool("Missiles");
  po->foreach([=](f::Poolable *p) {
    if (p->status()) {
      this->moveOneMissile((ecs::Node*)p,dt);
    }
  });
}


NS_END


