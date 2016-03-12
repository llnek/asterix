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
#include "Collide.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////
//
void Collide::preamble() {
  _aliens= _engine->getNodes("n/AlienSquad")[0];
  _player= _engine->getNodes("n/Ship")[0];
}

//////////////////////////////////////////////////////////////////////////
//
bool Collide::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::process(float dt) {
  // 1. get rid of all colliding bombs & missiles.
  //this.checkMissilesBombs();
  // 2. kill aliens
  checkMissilesAliens();

  // 3. ship ok?
  checkShipBombs();
  // 4. overruned by aliens ?
  checkShipAliens();
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::checkMissilesBombs() {

  auto mss = MGMS()->getPool("Missiles");
  auto bbs = MGMS()->getPool("Bombs");
  auto m = mss->ls();
  auto b= bbs->ls();

  F__LOOP(it, m) {
    auto e= (ecs::Node*) *it;
    auto h= CC_GEC(f::CHealth,e,"f/CHealth");
    auto s= CC_GEC(f::CDraw,e,"f/CDraw");
    if (e->status())
      F__LOOP(it2, b) {
        auto e2 = (ecs::Node*) *it2;
        auto h2= CC_GEC(f::CHealth,e2,"f/CHealth");
        auto s2= CC_GEC(f::CDraw,e2,"f/CDraw");
        if (e2->status() && cx::collide(s2,s)) {
          h2->hurt();
          h->hurt();
          break;
        }
      }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::checkMissilesAliens() {

  auto sqad= CC_GEC(AlienSquad, _aliens, "n/AlienSquad");
  auto mss = MGMS()->getPool("Missiles");
  auto c = sqad->aliens->ls();
  auto c2 = mss->ls();

  F__LOOP(it, c2) {
    auto e = (ecs::Node*) *it;
    auto h= CC_GEC(f::CHealth,e,"f/CHealth");
    auto s= CC_GEC(f::CDraw,e,"f/CDraw");
    if (e->status())
      F__LOOP(it2, c) {
        auto e2= (ecs::Node*) *it2;
        auto h2= CC_GEC(f::CHealth,e2,"f/CHealth");
        auto s2= CC_GEC(f::CDraw,e2,"f/CDraw");
        if (e2->status() &&
            cx::collide(s,s2)) {
          h2->hurt();
          h->hurt();
          break;
        }
      }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::checkShipBombs() {

  auto h2= CC_GEC(f::CHealth,_player,"f/CHealth");
  auto p= CC_GEC(f::CDraw, _player, "n/Ship");
  auto bbs= MGMS()->getPool("Bombs");
  auto c= bbs->ls();

  if (_player->status())
    F__LOOP(it, c) {
      auto b = (ecs::Node*) *it;
      auto h= CC_GEC(f::CHealth,b,"f/CHealth");
      auto bs= CC_GEC(f::CDraw, b, "f/CDraw");
      if (b->status() &&
          cx::collide(p, bs)) {
        h2->hurt();
        h->hurt();
        break;
      }
    }
}

//////////////////////////////////////////////////////////////////////////
//
void Collide::checkShipAliens() {

  auto sqad= CC_GEC(AlienSquad, _aliens, "n/AlienSquad");
  auto h2= CC_GEC(f::CHealth,_player,"f/CHealth");
  auto p = CC_GEC(f::CDraw, _player, "n/Ship");
  auto c = sqad->aliens->ls();
  auto sz= c.size();

  if (_player->status())
    F__LOOP(it, c) {
      auto a = (ecs::Node*) *it;
      auto h= CC_GEC(f::CHealth,a,"f/CHealth");
      auto s= CC_GEC(f::CDraw,a,"f/CDraw");
      if (a->status() &&
          cx::collide(p, s)) {
        h2->hurt();
        h->hurt();
        break;
      }
    }
}



NS_END


