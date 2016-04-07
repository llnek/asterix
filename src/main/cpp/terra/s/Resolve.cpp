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
  auto m= CC_GEC(f::CPixie, node,"f/CPixie");
  auto pe= MGMS()->getPool("HitEffects");
  auto e= (ecs::Node*)pe->take(true);
  auto pos= m->getPosition();

  cx::resurrect(e, pos.x, pos.y);
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::checkMissiles() {
  auto po = MGMS()->getPool("Missiles");
  auto box= MGMS()->getEnclosureBox();

  po->foreach([=](f::Poolable *p) {
    if (p->status()) {
      auto h=CC_GEC(f::CHealth,p,"f/CHealth");
      auto m=CC_GEC(Missile,p,"f/CPixie");
      auto pos= m->getPosition();
      if (!h->alive() ||
          !cx::pointInBox(box, pos.x, pos.y)) {
        this->onBulletDeath(PCAST(ecs::Node,p));
        cx::hibernate((ecs::Node*)p);
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
      auto h=CC_GEC(f::CHealth,p,"f/CHealth");
      auto b=CC_GEC(Missile,p,"f/CPixie");
      auto pos= b->getPosition();
      if (!h->alive() ||
          !cx::pointInBox(box, pos)) {
        this->onBulletDeath(PCAST(ecs::Node,p));
        cx::hibernate((ecs::Node*)p);
      }
    }
  });
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::onEnemyDeath(ecs::Node *alien) {
  auto ui=CC_GEC(f::CPixie,alien,"f/CPixie");
  auto pe= MGMS()->getPool("Explosions");
  auto ps= MGMS()->getPool("Sparks");
  auto pos= ui->getPosition();
  auto e= (ecs::Node*)pe->take(true);
  auto k= (ecs::Node*)ps->take(true);

  cx::resurrect(e,pos.x, pos.y);
  cx::resurrect(k,pos.x, pos.y);
  cx::sfxPlay("explodeEffect");
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::onShipDeath(ecs::Node *ship) {
  auto ui= CC_GEC(f::CPixie,ship,"f/CPixie");
  auto pe= MGMS()->getPool("Explosions");
  auto pos= ui->getPosition();
  auto e= (ecs::Node*)pe->take(true);
  cx::resurrect(e,pos.x, pos.y);
  cx::sfxPlay("shipDestroyEffect");
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::checkAliens() {
  auto box= MGMS()->getEnclosureBox();
  auto po= MGMS()->getPool("Baddies");

  po->foreach([=](f::Poolable *p) {
    if (p->status()) {
      auto h=CC_GEC(f::CHealth,p,"f/CHealth");
      auto ui=CC_GEC(f::CPixie,p,"f/CPixie");
      auto z=CC_GEC(Enemy,p,"n/Enemy");
      auto pos= ui->getPosition();
      if (!h->alive() ||
          !cx::pointInBox(box, pos)) {
        this->onEnemyDeath(PCAST(ecs::Node,p));
        cx::hibernate((ecs::Node*)p);
        auto msg = j::json({
            {"score", z->value} });
        SENDMSGEX("/game/players/earnscore", &msg);
      }
    }
  });
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::checkShip() {
  auto h = CC_GEC(f::CHealth, _ship, "f/CHealth");
  auto sp = CC_GEC(f::CPixie, _ship, "f/CPixie");
  if (_ship->status() &&
      !h->alive()) {
    this->onShipDeath(_ship);
    cx::hibernate(_ship);
    SENDMSG("/game/players/killed");
  }
}



NS_END



