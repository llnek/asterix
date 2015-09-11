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

#include "Resolve.h"
NS_USING(ash)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////
//
Resolve::Resolve(options)
  : state(options) {
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve;:RemoveFromEngine(Engine* e) {
  aliens= nullptr;
  ships= nullptr;
  engine=nullptr;
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::AddToEngine(Engine* e) {
  aliens= e->GetNodeList(AlienMotionNode::TypeId());
  ships= e->GetNodeList(ShipMotionNode::TypeId());
  engine=e;
}

//////////////////////////////////////////////////////////////////////////
//
bool Resolve::Update(float dt) {
  auto enemies = aliens->head;
  auto ship = ships->head;

  CheckMissiles();
  CheckBombs();
  CheckAliens(enemies);
  CheckShip(ship);
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::CheckMissiles() {
  auto cfg = XConfig::GetInstance();
  auto mss = cfg->GetPool("missiles");
  auto ht = CCSX::VisRect().height;
  auto c = mss->elements();

  for (auto it = c.begin(); it != c.end(); ++it) {
    auto m = *it;
    if (m->status) {
      if (m->Pos().y >= ht ||
          m->health <= 0) {
        m->Deflate();
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::CheckBombs() {
  auto cfg = XConfig::GetInstance();
  auto bbs = cfg->GetPool("bombs");
  auto c = bbs->elements();
  int bt = 0;

  for (auto it = c.begin(); it != c.end(); ++it) {
    auto b = *it;
    if (b->status) {
      if (b->health <= 0 ||
          b->Pos().y <= bt) {
        b->Deflate();
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::CheckAliens(Node* node) {
  auto sqad= node->aliens;
  auto c= sqad->aliens->pool->elements();

  for (auto it = c.begin(); it != c.end(); ++it) {
    auto en= *it;
    if (en->status) {
      if (en->health <= 0) {
        MainGame::Get()->EarnScore(en->value);
        en->Deflate();
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::CheckShip(Node* node) {
  auto ship = node->ship;

  if (ship->status &&
      ship->health <= 0) {
    ship->Deflate();
    MainGame::Get()->PlayerKilled();
  }
}


NS_END(invaders)

