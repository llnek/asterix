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

#include "support/Primitives.h"
#include "support/CCSX.h"
#include "2d/MainGame.h"
#include "Factory.h"
#include "Stager.h"
NS_ALIAS(cx, fusilli::ccsx)
NS_ALIAS(f, fusilli)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
Stager* Stager::Create(Factory* f, cc::Dictionary* d) {
  auto s = new Stager();
  s->Set(f,d);
  return s;
}

//////////////////////////////////////////////////////////////////////////
//
Stager::Stager()
  : ships(nullptr)
  , inited(false) {

}

//////////////////////////////////////////////////////////////////////////
//
Stager::~Stager() {
}

//////////////////////////////////////////////////////////////////////////
//
void Stager::RemoveFromEngine(a::Engine* e) {
  ships=nullptr;
}

//////////////////////////////////////////////////////////////////////////
//
void Stager::AddToEngine(a::Engine* e) {
  ships = e->GetNodeList(ShipMotionNode::TypeId());
}

//////////////////////////////////////////////////////////////////////////
//
void Stager::InitAlienSize() {
  auto z= cx::CSize("purple_bug_0.png");
  //pick purple since it is the largest
  state->setObject(f::Size2::create(z.width,z.height), "alienSize");
}

//////////////////////////////////////////////////////////////////////////
//
void Stager::InitShipSize() {
  auto z= cx::CSize("ship_0.png");
  state->setObject(f::Size2::create(z.width, z.height), "shipSize");
}

//////////////////////////////////////////////////////////////////////////
//
bool Stager::Update(float dt) {
  if (cx::IsTransitioning()) { return false; }
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

  cx::OnTouchOne(this);
  cx::OnMouse(this);
  g->PKInput(engine);
}

//////////////////////////////////////////////////////////////////////////
//
void Stager::Fire(t, evt) {
  auto s = f::MainGame::GetS();

  if ('/touch/one/move' === t ||
      '/mouse/move' === t) {} else {
    return;
  }
  if (s->IsOperational() &&
      NNP(ships->head)) {

    auto ship = ships->head->ship;
    auto pos = ship->Pos();
    auto x=pos.x;
    auto y=pos.y;
    auto wz= cx::VisRect();

    pos.add(cc::Vec2(evt->getDelta().x, 0));
    pos.clamp(cc::Vec2(0, 0),
              cc::Vec2(wz.width, wz.height));
    ship->setPos(pos.x, pos.y);
  }
}



NS_END(invaders)

