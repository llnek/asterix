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

#include "x2d/MainGame.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "n/gnodes.h"
#include "Resolve.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
Resolve::Resolve(not_null<Factory*> f, not_null<c::Dictionary*> options) {
  Init();
  Set(f, options);
}

//////////////////////////////////////////////////////////////////////////////
//
Resolve::~Resolve() {

}

//////////////////////////////////////////////////////////////////////////////
//
Resolve::Resolve() {
  Init();
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::Init() {
  SNPTR(aliens)
  SNPTR(ships)
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::RemoveFromEngine(not_null<a::Engine*> e) {
  SNPTR(aliens)
  SNPTR(ships)
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::AddToEngine(not_null<a::Engine*> e) {
  AlienMotionNode a;
  ShipMotionNode s;

  aliens= e->GetNodeList(a.TypeId());
  ships= e->GetNodeList(s.TypeId());
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
  auto cfg = f::XConfig::Self();
  auto mss = cfg->GetPool("missiles");
  auto ht = cx::VisRect().size.height;
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
  auto cfg = f::XConfig::Self();
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
void Resolve::CheckAliens(not_null<a::Node*> node) {
  auto sqad= a::NodeFld<AlienSquad>(node, "aliens");
  auto c= sqad->Elements();

  for (auto it = c.begin(); it != c.end(); ++it) {
    auto en= *it;
    if (en->status) {
      if (en->health <= 0) {
//        f::MainGame::Get()->EarnScore(en->value);
        f::MainGame::Get()->SendMsg("earnscore", en);
        en->Deflate();
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::CheckShip(not_null<a::Node*> node) {
  auto ship = a::NodeFld<Ship>(node, "ship");

  if (ship->status &&
      ship->health <= 0) {
    ship->Deflate();
    f::MainGame::Get()->SendMsg("PlayerKilled", nullptr);
  }
}


NS_END(invaders)

