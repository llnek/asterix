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

#include "base/CCEventKeyboard.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "x2d/MainGame.h"
#include "n/gnodes.h"
#include "Motions.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
Motions::Motions(not_null<Factory*> f, not_null<c::Dictionary*> d) {
  Init();
  Set(f,d);
}

//////////////////////////////////////////////////////////////////////////
//
Motions::Motions() {
  Init();
}

//////////////////////////////////////////////////////////////////////////
//
Motions::~Motions() {
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::Init() {
  SNPTR(cannons)
  SNPTR(aliens)
  SNPTR(ships)
  right=false;
  left= false;
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::RemoveFromEngine(not_null<a::Engine*> e) {
  SNPTR(aliens)
  SNPTR(ships)
  SNPTR(cannons)
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::AddToEngine(not_null<a::Engine*> e) {
  AlienMotionNode a;
  ShipMotionNode s;
  CannonCtrlNode c;

  aliens = e->GetNodeList(a.TypeId());
  ships = e->GetNodeList(s.TypeId());
  cannons = e->GetNodeList(c.TypeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Motions::Update(float dt) {
  auto enemy = aliens->head;
  auto ship=ships->head;
  auto cns= cannons->head;

  if (f::MainGame::Self()->IsRunning()) {
    if (NNP(enemy)) {
      ProcessAlienMotions(enemy,dt);
    }
    if (NNP(cns)) {
      ControlCannon(cns,dt);
    }
    if (NNP(ship)) {
      ScanInput(ship,dt);
    }
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::ControlCannon(not_null<a::Node*> node, float dt) {

  auto gun = a::NodeFld<Cannon>(node, "cannon");
  auto lpr= a::NodeFld<Looper>(node, "looper");
  auto ship= a::NodeFld<Ship>(node, "ship");
  auto t= lpr->timer0;

  if (! gun->hasAmmo) {
    //throttle the cannon with timer
    if (cx::TimerDone(t)) {
      ship->sprite->setSpriteFrame(ship->frame0);
      gun->hasAmmo=true;
      cx::UndoTimer(t);
      SNPTR(lpr->timer0)
    }
  } else {
    //TODO:
    if (f::MainGame::Get()->KeyPoll(c::EventKeyboard::KeyCode::KEY_SPACE)) {
      FireMissile(node,dt);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::FireMissile(not_null<a::Node*> node, float dt) {

  auto gun= a::NodeFld<Cannon>(node, "cannon");
  auto lpr= a::NodeFld<Looper>(node, "looper");
  auto ship=a::NodeFld<Ship>(node, "ship");

  auto cfg= f::XConfig::Self();
  auto top= cx::GetTop(ship->sprite);
  auto p= cfg->GetPool("missiles");
  auto pos= ship->Pos();
  auto ent= p->Get();

  if (ENP(ent)) {
    factory->CreateMissiles(36);
    ent= p->Get();
  }

  ent->Inflate(pos.x, top+4);

  lpr->timer0 = cx::CreateTimer(
    f::MainGame::Get(), gun->coolDownWindow);
  gun->hasAmmo=false;
  ship->sprite->setSpriteFrame(ship->frame1);

  cx::SfxPlay("ship-missile");
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::ScanInput(not_null<a::Node*> node, float dt) {

  auto m= a::NodeFld<Motion>(node, "motion");
  auto s= a::NodeFld<Ship>(node, "ship");

  if (f::MainGame::Get()->KeyPoll(
      c::EventKeyboard::KeyCode::KEY_RIGHT_ARROW)) {
    m->right=true;
  }
  if (f::MainGame::Get()->KeyPoll(
      c::EventKeyboard::KeyCode::KEY_LEFT_ARROW)) {
    m->left=true;
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::ProcessAlienMotions(not_null<a::Node*> node, float dt) {

  auto sqad= a::NodeFld<AlienSquad>(node, "aliens");
  auto lpr = a::NodeFld<Looper>(node, "looper");
  auto g= f::MainGame::Get();

  if (ENP(lpr->timer0)) {
    lpr->timer0= cx::CreateTimer(g,1);
  }

  if (ENP(lpr->timer1)) {
    lpr->timer1= cx::CreateTimer(g,2);
  }
}




NS_END(invaders)


