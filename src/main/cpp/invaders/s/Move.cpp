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

#include "Move.h"
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////
//
Move::Move(options)
  : state(options) {
}

//////////////////////////////////////////////////////////////////////////
//
void Move::RemoveFromEngine(Engine* e) {
  ships=nullptr;
}

//////////////////////////////////////////////////////////////////////////
//
void Move::AddToEngine(Engine* e) {
  ships = e->GetNodeList(ShipMotionNode::TypeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Move::Update(float dt) {
  auto node = ships->head;
  if (MainGame::Get()->IsOperational()) {
    if (NNP(node)) {
      ProcessShipMotions(node, dt);
    }
    MoveMissiles(dt);
    MoveBombs(dt);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Move::ProcessShipMotions(Node* node, float dt) {
  auto motion = node->motion;
  auto sv = node->velocity;
  auto ship= node->ship;
  auto pos = ship->Pos();
  auto x= Pos().x;
  auto y= Pos().y;

  if (motion->right) {
    x = pos.x + dt * sv.vel.x;
  }

  if (motion->left) {
    x = pos.x - dt * sv.vel.x;
  }

  ship->setPos(x,y);
  Clamp(ship);

  motion->right=false;
  motion->left=false;
}

//////////////////////////////////////////////////////////////////////////
//
void Move::Clamp(ComObj* ship) {
  auto sz= ship->sprite->getContentSize();
  auto cfg = XConfig::GetInstance();
  auto tile= SCAST(Integer*, cfg->GetCst("TILE"))->getValue();
  auto pos= ship->Pos();
  auto wz = CCSX::VisRect();

  if (CCSX::GetRight(ship->sprite) > wz.width - tile) {
    ship->setPos(wz.width - tile - sz.width * 0.5, pos.y);
  }
  if (CCSX::GetLeft(ship->sprite) < tile) {
    ship->setPos(tile + sz.width * 0.5, pos.y);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Move::MoveBombs(float dt) {
  auto cfg = XConfig::GetInstance();
  auto bbs= cfg->GetPool("bombs");
  auto c= bbs->elements();

  for (auto it= c.begin(); it != c.end(); ++it) {
    auto b = *it;
    if (b->status) {
      auto pos= b->Pos();
      auto y = pos.y + dt * b.vel.y;
      b->setPos(pos.x, y);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Move::MoveMissiles(float dt) {
  auto cfg = XConfig::GetInstance();
  auto bbs= cfg->GetPool("missiles");
  auto c= mss->elements();

  for (auto it= c.begin(); it != c.end(); ++it) {
    auto m = *it;
    auto pos= m->Pos();
    auto y = pos.y + dt * m->vel.y;
    m->setPos(pos.x, y);
  }
}

NS_END(invaders)



