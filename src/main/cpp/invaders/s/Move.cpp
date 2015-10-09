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
#include "x2d/MainGame.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Move.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
Move::Move(not_null<Factory*> f, not_null<c::Dictionary*> d) {
  SNPTR(ships)
  Set(f,d);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Move::~Move() {
}

//////////////////////////////////////////////////////////////////////////
//
void Move::RemoveFromEngine(not_null<a::Engine*> e) {
  SNPTR(ships)
}

//////////////////////////////////////////////////////////////////////////
//
void Move::AddToEngine(not_null<a::Engine*> e) {
  CCLOG("adding system: Move");
  ShipMotionNode s;
  ships = e->GetNodeList(s.TypeId());
  CCLOG("added system: Move");
}

//////////////////////////////////////////////////////////////////////////
//
bool Move::OnUpdate(float dt) {
  auto node = ships->head;
  if (MGMS()->IsRunning()) {
    if (NNP(node)) {
      ProcessShipMotions(node, dt);
    }
    MoveMissiles(dt);
    MoveBombs(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Move::ProcessShipMotions(not_null<a::Node*> node, float dt) {

  auto motion = a::NodeFld<Motion>(node,"motion");
  auto sv = a::NodeFld<Velocity>(node,"vel");
  auto ship= a::NodeFld<Ship>(node,"ship");

  auto pos = ship->Pos();
  auto x= pos.x;
  auto y= pos.y;

//  CCLOG("motion right = %s", motion->right ? "true" : "false");
//  CCLOG("motion left = %s", motion->left ? "true" : "false");

  if (motion->right) {
    x = pos.x + dt * sv->x;
  }

  if (motion->left) {
    x = pos.x - dt * sv->x;
  }

  ship->SetPos(x,y);
  Clamp(ship);

  motion->right=false;
  motion->left=false;

}

//////////////////////////////////////////////////////////////////////////
//
void Move::Clamp(not_null<Ship*> ship) {

  auto tile= f::CstVal<c::Integer>("TILE")->getValue();
  auto sz= ship->sprite->getContentSize();
  auto pos= ship->Pos();
  auto wz = cx::VisRect();

  if (cx::GetRight(ship->sprite) > wz.size.width - tile) {
    ship->SetPos(wz.size.width - tile - sz.width * 0.5, pos.y);
  }
  if (cx::GetLeft(ship->sprite) < tile) {
    ship->SetPos(tile + sz.width * 0.5, pos.y);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Move::MoveBombs(float dt) {

  auto bbs= MGMS()->GetPool("bombs");
  auto c= bbs->Elements();

  for (auto it= c.begin(); it != c.end(); ++it) {
    auto b = *it;
    if (b->status) {
      auto pos= b->Pos();
      auto y = pos.y + dt * b->vel.y;
      b->SetPos(pos.x, y);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Move::MoveMissiles(float dt) {

  auto mss= MGMS()->GetPool("missiles");
  auto c= mss->Elements();

  for (auto it= c.begin(); it != c.end(); ++it) {
    auto m = *it;
    auto pos= m->Pos();
    auto y = pos.y + dt * m->vel.y;
    m->SetPos(pos.x, y);
  }
}



NS_END(invaders)



