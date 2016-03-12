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
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Move.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////
//
void Move::preamble() {
  _player = _engine->getNodes("n/Ship")[0];
}

//////////////////////////////////////////////////////////////////////////
//
bool Move::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Move::process(float dt) {
  processShipMotions( dt);
  moveMissiles(dt);
  moveBombs(dt);
}

//////////////////////////////////////////////////////////////////////////
//
void Move::processShipMotions( float dt) {

  auto mv= CC_GEC(f::CMove, _player,"f/CMove");
  auto sp= CC_GEC(Ship, _player,"n/Ship");
  auto pos = sp->pos();
  auto x= pos.x;
  auto y= pos.y;

  if (MGML()->keyPoll(KEYCODE::KEY_RIGHT_ARROW)) {
    x = pos.x + dt * mv->speed;
  }

  if (MGML()->keyPoll(KEYCODE::KEY_LEFT_ARROW)) {
    x = pos.x - dt * mv->speed;
  }

  sp->setPos(x,y);
  clamp(sp);
}

//////////////////////////////////////////////////////////////////////////
//
void Move::clamp(Ship *ship) {

  auto tile= CC_CSV(c::Float,"TILE");
  auto wz = cx::visRect();
  auto sz= ship->csize();
  auto pos= ship->pos();

  if (cx::getRight(ship->node) > wz.size.width - tile) {
    ship->setPos(wz.size.width - tile - HWZ(sz), pos.y);
  }
  if (cx::getLeft(ship->node) < tile) {
    ship->setPos(tile + HWZ(sz), pos.y);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Move::moveBombs(float dt) {

  auto bbs= MGMS()->getPool("Bombs");
  auto c= bbs->ls();

  F__LOOP(it, c) {
    auto b = (ecs::Node*) *it;
    if (b->status()) {
      auto mv= CC_GEC(f::CMove,b,"f/CMove");
      auto s= CC_GEC(f::CDraw,b,"f/CDraw");
      auto pos= s->pos();
      auto y = pos.y + dt * mv->speed;
      s->setPos(pos.x, y);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Move::moveMissiles(float dt) {

  auto mss= MGMS()->getPool("Missiles");
  auto c= mss->ls();

  F__LOOP(it, c) {
    auto e = (ecs::Node*) *it;
    if (e->status()) {
      auto mv= CC_GEC(f::CMove,e,"f/CMove");
      auto s= CC_GEC(f::CDraw,e,"f/CDraw");
      auto pos= s->pos();
      auto y = pos.y + dt * mv->speed;
      s->setPos(pos.x, y);
    }
  }
}



NS_END



