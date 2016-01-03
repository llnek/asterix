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
#include "EFactory.h"
#include "Resolve.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////
//
void Resolve::addToEngine(not_null<a::Engine*> e) {
  AlienMotionNode a;
  ShipMotionNode s;

  aliens= e->getNodeList(a.typeId());
  ships= e->getNodeList(s.typeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Resolve::update(float dt) {
  auto enemies = aliens->head;
  auto ship = ships->head;

  checkMissiles();
  checkBombs();
  checkAliens(enemies);
  checkShip(ship);

  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::checkMissiles() {

  auto mss = MGMS()->getPool("missiles");
  auto ht = cx::visRect().size.height;
  auto c = mss->list();

  F__LOOP(it, c) {
    auto &m = *it;
    if (m->status) {
      if (m->pos().y >= ht ||
          m->health <= 0) {
        m->deflate();
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::checkBombs() {

  auto bbs = MGMS()->getPool("bombs");
  auto c = bbs->list();
  int bt = 0;

  F__LOOP(it, c) {

    auto &b = *it;
    if (b->status) {
      if (b->health <= 0 ||
          b->pos().y <= bt) {
        b->deflate();
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::checkAliens(a::Node *node) {
  auto sqad= CC_GNF(AlienSquad, node, "aliens");
  auto c= sqad->list();

  F__LOOP(it, c) {

    auto &en= *it;
    if (en->status) {
      if (en->health <= 0) {
        auto msg= j::json({
              {"score", en->score }
            });
        SENDMSGEX("/game/player/earnscore", &msg);
        en->deflate();
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::checkShip(a::Node *node) {
  auto ship = CC_GNF(Ship, node, "ship");

  if (ship->status &&
      ship->health <= 0) {
    ship->deflate();
    SENDMSG("/game/player/killed");
  }
}

NS_END(invaders)

