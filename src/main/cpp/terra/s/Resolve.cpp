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
#include "core/CCSX.h"
#include "Resolve.h"
#include "Game.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::preamble() {
  ArenaNode a;
  ShipNode n;

  arena = engine->getNodeList(a.typeId());
  ship = engine->getNodeList(n.typeId());
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
void Resolve::onBulletDeath(f::ComObj *b) {
  auto pe= MGMS()->getPool("HitEffects");
  auto pos= b->pos();
  auto e= pe->get();

  if (ENP(e)) {
    SCAST(GEngine*,engine)->createHitEffects();
    e= pe->get();
  }

  e->inflate(pos.x, pos.y);
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::checkMissiles() {
  auto box= MGMS()->getEnclosureBox();
  auto p = MGMS()->getPool("Missiles");

  p->foreach([=](f::ComObj *m) {
    if (m->status) {
      auto pos= m->pos();
      if (m->HP <= 0 ||
          !cx::pointInBox(box, pos.x, pos.y)) {
        this->onBulletDeath(m);
        m->deflate();
      }
    }
  });
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::checkBombs() {
  auto box= MGMS()->getEnclosureBox();
  auto p = MGMS()->getPool("Bombs");

  p->foreach([=](f::ComObj *b) {
    if (b->status) {
      auto pos= b->pos();
      if (b->HP <= 0 ||
          !cx::pointInBox(box, pos)) {
        this->onBulletDeath(b);
        b->deflate();
      }
    }
  });
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::onEnemyDeath(f::ComObj *alien) {
  auto pe= MGMS()->getPool("Explosions");
  auto ps= MGMS()->getPool("Sparks");
  auto pos= alien->pos();
  auto e= pe->get();
  auto s= ps->get();
  if (ENP(e)) {
    SCAST(GEngine*,engine)->createExplosions();
    e= pe->get();
  }
  e->inflate(pos.x, pos.y);
  if (ENP(s)) {
    SCAST(GEngine*,engine)->createSparks();
    s=ps->get();
  }
  s->inflate( pos.x, pos.y);
  cx::sfxPlay("explodeEffect");
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::onShipDeath(f::ComObj *ship) {
  auto pe= MGMS()->getPool("Explosions");
  auto pos= ship->pos();
  auto e= pe->get();

  if (ENP(e)) {
    SCAST(GEngine*,engine)->createExplosions();
    e= pe->get();
  }

  e->inflate( pos.x, pos.y);
  cx::sfxPlay("shipDestroyEffect");
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::checkAliens() {
  auto box= MGMS()->getEnclosureBox();
  auto p= MGMS()->getPool("Baddies");

  p->foreach([=](f::ComObj *a) {
    if (a->status) {
      auto pos= a->pos();
      if (a->HP <= 0 ||
          !cx::pointInBox(box, pos)) {
        this->onEnemyDeath(a);
        a->deflate();
        auto msg = j::json({ {"score", a->score} });
        SENDMSGEX("/game/players/earnscore", &msg);
      }
    }
  });
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::checkShip() {
  auto sp = CC_GNLF(Ship, ship, "ship");
  if (sp->status) {
    if (sp->HP <= 0) {
      this->onShipDeath(sp);
      sp->deflate();
      SENDMSG("/game/players/killed");
    }
  }
}



NS_END(terra)



