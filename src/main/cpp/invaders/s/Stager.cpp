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

#include "Stager.h"


//////////////////////////////////////////////////////////////////////////
//
Stager::Stager(options)
  : state(options),
  ships(nullptr),
  inited(false) {
}

//////////////////////////////////////////////////////////////////////////
//
Stager::~Stager() {
}

//////////////////////////////////////////////////////////////////////////
//
void Stager::RemoveFromEngine(Engine* e) {
  ships=nullptr;
}

//////////////////////////////////////////////////////////////////////////
//
void Stager::AddToEngine(Engine* e) {
  ships = e->GetNodeList(ShipMotionNode::TypeId());
}

//////////////////////////////////////////////////////////////////////////
//
void Stager::InitAlienSize() {
  auto z= CCSX::CSize("purple_bug_0.png");
  //pick purple since it is the largest
  state->setObject(Size2::create(z.width,z.height), "alienSize");
}

//////////////////////////////////////////////////////////////////////////
//
void Stager::InitShipSize() {
  auto z= CCSX::CSize("ship_0.png");
  state->setObject(Size2::create(z.width, z.height), "shipSize");
}

//////////////////////////////////////////////////////////////////////////
//
bool Stager::Update(float dt) {
  if (CCSX::IsTransitioning()) { return false; }
  if (! inited) {
    OnceOnly();
    inited=true;
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Stager::OnceOnly() {
  auto g = MainGame::Get();

  InitAlienSize();
  InitShipSize();

  factory->CreateMissiles();
  factory->CreateBombs();
  factory->CreateExplosions();

  factory->CreateAliens();
  factory->CreateShip();

  CCSX::OnTouchOne(this);
  CCSX::OnMouse(this);
  g->PKInput(engine);
}

//////////////////////////////////////////////////////////////////////////
//
void Stager::Fire(t, evt) {
  auto g = MainGame::Get();

  if ('/touch/one/move' === t ||
      '/mouse/move' === t) {} else {
    return;
  }
  if (g->IsOperational() &&
      NNP(ships->head)) {
    auto ship = ships->head->ship;
    auto pos = ship->Pos();
    auto x=pos.x;
    auto y=pos.y;
    auto wz= CCSX::VisRect();
    pos.add(Vec2(evt->getDelta().x, 0));
    pos.clamp(Vec2(0, 0),
              Vec2(wz.width, wz.height));
    ship->setPos(pos.x, pos.y);
  }
}


NS_END(invaders)
