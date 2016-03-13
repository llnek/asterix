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
#include "Resolve.h"
#include "Game.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::preamble() {
  _ship = _engine->getNodes("f/CGesture")[0];
  _arena = _engine->getNodes("n/GVars")[0];
}

//////////////////////////////////////////////////////////////////////////////
//
bool Resolve::update(float dt) {

  if (MGMS()->isLive()) {
    checkMissiles();
    checkBombs();
    checkAliens();
    checkShip();
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::onBulletDeath(ecs::Node *node) {
  auto m= CC_GEC(f::CDraw, node,"f/CDraw");
  auto pe= MGMS()->getPool("HitEffects");
  auto e= pe->getAndSet(true);
  auto pos= m->pos();
  e->inflate(pos.x, pos.y);
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::checkMissiles() {
  auto po = MGMS()->getPool("Missiles");
  auto box= MGMS()->getEnclosureBox();

  po->foreach([=](f::Poolable *p) {
    if (p->status()) {
      auto e=(ecs::Node*)p;
      auto m=CC_GEC(Missile,e,"f/CDraw");
      auto pos= m->pos();
      if (m->HP <= 0 ||
          !cx::pointInBox(box, pos.x, pos.y)) {
        this->onBulletDeath(e);
        m->deflate();
        e->yield();
      }
    }
  });
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::checkBombs() {
  auto box= MGMS()->getEnclosureBox();
  auto po = MGMS()->getPool("Bombs");

  po->foreach([=](f::Poolable *p) {
    if (p->status()) {
      auto e=(ecs::Node*)p
      auto b=CC_GEC(Bomb,e,"f/CDraw");
      auto pos= b->pos();
      if (b->HP <= 0 ||
          !cx::pointInBox(box, pos)) {
        this->onBulletDeath(e);
        b->deflate();
        e->yield();
      }
    }
  });
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::onEnemyDeath(ecs::Node *alien) {
  auto ui=CC_GEC(f::CDraw,alien,"f/CDraw");
  auto pe= MGMS()->getPool("Explosions");
  auto ps= MGMS()->getPool("Sparks");
  auto pos= ui->pos();
  auto e= pe->getAndSet(true);
  auto s= ps->getAndSet(true);
  e->inflate(pos.x, pos.y);
  s->inflate(pos.x, pos.y);
  cx::sfxPlay("explodeEffect");
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::onShipDeath(ecs::Node *ship) {
  auto ui= CC_GEC(f::CDraw,ship,"f/CDraw");
  auto pe= MGMS()->getPool("Explosions");
  auto pos= ui->pos();
  auto e= pe->getAndSet(true);
  e->inflate(pos.x, pos.y);
  cx::sfxPlay("shipDestroyEffect");
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::checkAliens() {
  auto box= MGMS()->getEnclosureBox();
  auto po= MGMS()->getPool("Baddies");

  po->foreach([=](f::Poolable *p) {
    if (p->status()) {
      auto e=(ecs::Node*)p
      auto ui=CC_GEC(f::CDraw,e,"f/CDraw");
      auto h=CC_GEC(f::CHealth,e,"f/CHealth");
      auto z=CC_GEC(Enemy,e,"n/Enemy");
      auto pos= ui->pos();
      if (!h->alive() ||
          !cx::pointInBox(box, pos)) {
        this->onEnemyDeath(e);
        ui->deflate();
        e->yield();
        auto msg = j::json({
            {"score", z.enemyType.scoreValue} });
        SENDMSGEX("/game/players/earnscore", &msg);
      }
    }
  });
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::checkShip() {
  auto h = CC_GEC(f::CHealth, _ship, "f/CHealth");
  auto sp = CC_GEC(f::CDraw, _ship, "f/CDraw");
  if (_ship->status() &&
      !h->alive()) {
      this->onShipDeath(_ship);
      sp->deflate();
      _ship->yield();
      SENDMSG("/game/players/killed");
  }
}



NS_END



