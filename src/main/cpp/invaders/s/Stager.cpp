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

#include "core/Primitives.h"
#include "core/CCSX.h"
#include "x2d/MainGame.h"
#include "n/gnodes.h"
#include "Factory.h"
#include "Stager.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
Stager::Stager(not_null<Factory*> f, not_null<c::Dictionary*> d) {
  inited=false;
  SNPTR(ships)
  Set(f,d);
}

//////////////////////////////////////////////////////////////////////////
//
Stager::~Stager() {
  //printf("Stager dtor\n");
}

//////////////////////////////////////////////////////////////////////////
//
void Stager::RemoveFromEngine(not_null<a::Engine*> e) {
  SNPTR(ships)
}

//////////////////////////////////////////////////////////////////////////
//
void Stager::AddToEngine(not_null<a::Engine*> e) {
  CCLOG("adding system: Stager");
  if (! inited) {
    OnceOnly(e);
    inited=true;
  }
  ShipMotionNode s;
  ships = e->GetNodeList(s.TypeId());
  CCLOG("added system: Stager");
}

//////////////////////////////////////////////////////////////////////////
//
void Stager::InitAlienSize() {
  auto z= cx::CalcSize("purple_bug_0.png");
  //pick purple since it is the largest
  state->setObject(f::Size2::create(z.width,z.height), "alienSize");
}

//////////////////////////////////////////////////////////////////////////
//
void Stager::InitShipSize() {
  auto z= cx::CalcSize("ship_0.png");
  state->setObject(f::Size2::create(z.width, z.height), "shipSize");
}

//////////////////////////////////////////////////////////////////////////
//
bool Stager::OnUpdate(float dt) {
  if (cx::IsTransitioning()) { return false; }
  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Stager::OnceOnly(a::Engine* e) {

  InitAlienSize();
  InitShipSize();

  factory->CreateExplosions();
  factory->CreateMissiles();
  factory->CreateBombs();
  factory->CreateAliens();
  factory->CreateShip();

  e->ForceSync();

  //cx::OnTouchOne(this);
  //cx::OnMouse(this);
  //TODO:
  //g->PKInput(engine);
}

//////////////////////////////////////////////////////////////////////////
//
void Stager::Fire(const stdstr& t, void* evt) {

  if ("/touch/one/move" == t ||
      "/mouse/move" == t) {} else {
    return;
  }
  if (MGMS()->IsRunning() && NNP(ships->head)) {

    auto ship = (f::ComObj*) ships->head->Get("ship");
    auto pos = ship->Pos();
    auto x=pos.x;
    auto y=pos.y;
    auto wz= cx::VisRect();

    //TODO:
    //pos.add(c::Vec2(evt->getDelta().x, 0));
    pos.clamp(c::Vec2(0, 0),
              c::Vec2(wz.size.width, wz.size.height));
    ship->SetPos(pos.x, pos.y);
  }
}



NS_END(invaders)

