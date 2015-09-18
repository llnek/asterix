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

#include "support/XConfig.h"
#include "support/CCSX.h"
#include "n/gnodes.h"
#include "Resolve.h"
NS_ALIAS(cx, fusilli::ccsx)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
Resolve* Resolve::Create(Factory* f, c::Dictionary* options) {
  auto s = new Resolve();
  s->Set(f, options);
  return s;
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve;:RemoveFromEngine(a::Engine* e) {
  aliens= nullptr;
  ships= nullptr;
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::AddToEngine(a::Engine* e) {
  aliens= e->GetNodeList(AlienMotionNode::TypeId());
  ships= e->GetNodeList(ShipMotionNode::TypeId());
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

  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::CheckMissiles() {
  auto cfg = f::XConfig::GetInstance();
  auto mss = cfg->GetPool("missiles");
  auto ht = cx::VisRect().height;
  auto c = mss->Elements();

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
  auto cfg = f::XConfig::GetInstance();
  auto bbs = cfg->GetPool("bombs");
  auto c = bbs->Elements();
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
void Resolve::CheckAliens(a::Node* node) {
  auto sqad= NodeFld<>(node, "aliens");
  auto c= sqad->Elements();

  for (auto it = c.begin(); it != c.end(); ++it) {
    auto en= *it;
    if (en->status) {
      if (en->health <= 0) {
        f::MainGame::Get()->EarnScore(en->value);
        en->Deflate();
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::CheckShip(a::Node* node) {
  auto ship = NodeFld<>(node, "ship");

  if (ship->status &&
      ship->health <= 0) {
    ship->Deflate();
    f::MainGame::Get()->PlayerKilled();
  }
}


NS_END(invaders)

