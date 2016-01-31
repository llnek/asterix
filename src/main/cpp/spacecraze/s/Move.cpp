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

#pragma once

//////////////////////////////////////////////////////////////////////////////
#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "GEngine.h"
#include "Move.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(spacecraze)


//////////////////////////////////////////////////////////////////////////////
//
void Move::preamble() {
  aliens = engine->getNodeList(AlienNode().typeId());
  ships = engine->getNodeList(ShipNode().typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::processAliens(float dt) {

  auto squad = CC_GNLF(AlienSquad,aliens,"squad");
  auto lpr = CC_GNLF(Looper,aliens,"looper");
  auto a = squad->aliens->randGet();

  if (cx::timerDone(lpr->timer)) {
    cx::undoTimer(lpr->timer);
    fireBombs((Alien*) a);
    lpr->timer=cx::reifyTimer(MGML(), lpr->duration);
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void Move::fireBombs(Alien* enemy) {

  auto scale_up = c::ScaleTo::create(0.25f, 1.0f);
  auto pos = c::ccpSub(enemy->pos(),
      c::ccp(0, enemy->csize().height * 0.3f));
  auto p= MGMS()->getPool("Bombs");
  auto b= p->get();

  if (ENP(b)) {
    SCAST(GEngine*,engine)->createBombs();
    b= p->get();
  }

  SCAST(Bomb*,b)->morph(enemy->type);
  b->inflate(pos.x, pos.y);
  b->sprite->setScale(0.5f);
  b->sprite->runAction(scale_up);

  cx::sfxPlay("shoot_enemy");
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::processShip(float dt) {
  auto lpr = CC_GNLF(Looper,ships,"looper");
  auto ship = CC_GNLF(Ship,ships,"ship");
  auto bx= MGMS()->getEnclosureBox();
  auto pos = ship->pos();
  auto dirty=false;
  auto x= pos.x;
  auto y= pos.y;

  if (ship->godMode) { return; }

  if (MGML()->keyPoll(KEYCODE::KEY_RIGHT_ARROW)) {
    x += ship->vel.x * dt;
    dirty=true;
  }

  if (MGML()->keyPoll(KEYCODE::KEY_LEFT_ARROW)) {
    x -= ship->vel.x * dt;
    dirty=true;
  }

  if (dirty) {
    ship->setPos(cx::clamp(
          c::Vec2(x,y), ship->csize(), bx).x, pos.y);
  }

  if (cx::timerDone(lpr->timer)) {
    cx::undoTimer(lpr->timer);
    firePlayerBullet(dt);
    lpr->timer= cx::reifyTimer(MGML(), lpr->duration);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::firePlayerBullet(float dt) {

  auto ship = CC_GNLF(Ship,ships,"ship");
  auto wb= cx::visBox();
  auto pos = c::ccpAdd(ship->pos(),
      c::ccp(0, ship->csize().height * 0.3f));

  auto scale_up = c::CCScaleTo::create(0.25f, 1.0f);
  auto p= MGMS()->getPool("Missiles");
  auto m = p->get();
  if (ENP(m)) {
    SCAST(GEngine*,engine)->createMissiles();
    m=p->get();
  }

  m->inflate(pos.x,pos.y);
  m->sprite->setScale(0.5f);
  m->sprite->runAction(scale_up);

  cx::sfxPlay("shoot_player");
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::processMissiles(float dt) {
  auto world = MGMS()->getEnclosureBox();
  auto p= MGMS()->getPool("Missiles");
  auto ms= p->list();
  F__LOOP(it,ms) {
    auto m = (Missile*) *it;
    if (m->status) {
      auto pos= m->pos();
      pos.y += m->vel.y * dt;
      if (pos.y > world.top) {
        m->deflate();
      } else {
        m->setPos(pos.x,pos.y);
      }
    }
  };
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::processBombs(float dt) {
  auto world = MGMS()->getEnclosureBox();
  auto p= MGMS()->getPool("Bombs");
  auto bs= p->list();
  F__LOOP(it,bs) {
    auto b = (Bomb*) *it;
    if (b->status) {
      auto pos= b->pos();
      pos.y -= b->vel.y * dt;
      if (pos.y < world.bottom) {
        b->deflate();
      } else {
        b->setPos(pos.x,pos.y);
      }
    }
  };
}

//////////////////////////////////////////////////////////////////////////////
//
bool Move::update(float dt) {
  if (MGMS()->isLive()) {
    processAliens(dt);
    processBombs(dt);
    processShip(dt);
    processMissiles(dt);
  }
  return true;
}



NS_END





