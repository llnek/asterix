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
  _aliens = _engine->getNodes("n/AlienSquad")[0];
  _ship = _engine->getNodes("f/CGesture")[0];
  _arena = _engine->getNodes("n/GVars")[0];
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::processAliens(float dt) {

  auto squad = CC_GEC(AlienSquad,_aliens,"n/AlienSquad");
  auto lpr = CC_GNLF(f::Loopers,_arena,"f/Loopers");
  auto po=MGMS()->getPool("Aliens");
  auto pa= po->actives();
  if (pa.size() == 0) { return; }
  auto idx = cx::randInt(pa.size());
  auto a= pa[idx];

  if (cx::timerDone(lpr->tms[1].timer)) {
    cx::undoTimer(lpr->tms[1].timer);
    fireBombs(a);
    lpr->tms[1].timer=cx::reifyTimer(MGML(), lpr->tms[1].duration);
  }

}

//////////////////////////////////////////////////////////////////////////////
//
void Move::fireBombs(ecs::Node *node) {

  auto scale_up = c::ScaleTo::create(0.25, 1.0);
  auto ui=CC_GEC(Alien,node,"f/CPixie");
  auto pos = c::ccpSub(ui->pos(),
      c::Vec2(0, ui->csize().height * 0.3));
  auto po= MGMS()->getPool("Bombs");
  auto e= po->take(true);
  auto h=CC_GEC(f::CHealth,e,"f/CHealth");
  auto b=CC_GEC(Bomb,e,"f/CPixie");
  b->morph(ui->type);
  b->inflate(pos.x, pos.y);
  h->reset();
  b->node->setScale(0.5);
  b->node->runAction(scale_up);

  cx::sfxPlay("shoot_enemy");
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::processShip(float dt) {
  auto lpr = CC_GEC(f::Loopers,_arena,"f/Loopers");
  auto mv = CC_GEC(f::CMove,_ship,"f/CMove");
  auto sp = CC_GEC(Ship,_ship,"f/CPixie");
  auto bx= MGMS()->getEnclosureBox();
  auto pos = sp->pos();
  auto dirty=false;
  auto x= pos.x;
  auto y= pos.y;

  if (_ship->isGod()) { return; }

  if (MGML()->keyPoll(KEYCODE::KEY_RIGHT_ARROW)) {
    x += mv->vel.x * dt;
    dirty=true;
  }

  if (MGML()->keyPoll(KEYCODE::KEY_LEFT_ARROW)) {
    x -= mv->vel.x * dt;
    dirty=true;
  }

  if (dirty) {
    sp->setPos(cx::clamp(
          c::Vec2(x,y), sp->csize(), bx).x, pos.y);
  }

  if (cx::timerDone(lpr->tms[0].timer)) {
    cx::undoTimer(lpr->tms[0].timer);
    firePlayerBullet(dt);
    lpr->tms[0].timer= cx::reifyTimer(MGML(), lpr->tms[0].duration);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::firePlayerBullet(Ship *sp, float dt) {

  auto scale_up = c::CCScaleTo::create(0.25, 1.0);
  auto pos = c::ccpAdd(sp->pos(),
      c::Vec2(0, sp->csize().height * 0.3));
  auto po= MGMS()->getPool("Missiles");
  auto m = po->take(true);

  auto h=CC_GEC(f::CHealth,m,"f/CHealth");
  auto ui=CC_GEC(f::CPixie,m,"f/CPixie");

  ui->inflate(pos.x,pos.y);
  h->reset();
  ui->node->setScale(0.5);
  ui->node->runAction(scale_up);

  cx::sfxPlay("shoot_player");
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::processMissiles(float dt) {
  auto world = MGMS()->getEnclosureBox();
  auto p= MGMS()->getPool("Missiles");
  auto ms= p->ls();

  F__LOOP(it,ms) {
    auto m = *it;
    if (m->status()) {
      auto ui=CC_GEC(f::CPixie,m,"f/CPixie");
      auto mv=CC_GEC(f::CMove,m,"f/CMove");
      auto pos= ui->pos();
      pos.y += mv->vel.y * dt;
      if (pos.y > world.top) {
        ui->deflate();
        m->yield();
      } else {
        ui->setPos(pos.x,pos.y);
      }
    }
  };
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::processBombs(float dt) {
  auto world = MGMS()->getEnclosureBox();
  auto po= MGMS()->getPool("Bombs");
  auto bs= po->ls();

  F__LOOP(it,bs) {
    auto b = *it;
    if (b->status()) {
      auto ui=CC_GEC(f::CPixie,b,"f/CPixie");
      auto mv=CC_GEC(f::CMove,b,"f/CMove");
      auto pos= ui->pos();
      pos.y -= mv->vel.y * dt;
      if (pos.y < world.bottom) {
        ui->deflate();
        b->yield();
      } else {
        ui->setPos(pos.x,pos.y);
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





