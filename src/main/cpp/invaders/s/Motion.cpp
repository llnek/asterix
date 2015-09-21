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
#include "Motion.h"
NS_ALIAS(cx, fusilli::ccsx)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
Motion* Motion::Create(Factory* f, c::Dictionary* d) {
  auto s = new Motion();
  s->Set(f,d);
  return s;
}

//////////////////////////////////////////////////////////////////////////
//
Motion::~Motion() {
}

//////////////////////////////////////////////////////////////////////////
//
Motion::Motion()
  : aliens(nullptr)
  , ships(nullptr)
  , right(false)
  , left(false)
  , cannons(nullptr) {
}

//////////////////////////////////////////////////////////////////////////
//
void Motion::RemoveFromEngine(a::Engine* e) {
  aliens = nullptr;
  ships = nullptr;
  cannons= nullptr;
}

//////////////////////////////////////////////////////////////////////////
//
void Motion::AddToEngine(a::Engine* e) {
  aliens = e->GetNodeList(AlienMotionNode::TypeId());
  ships = e->GetNodeList(ShipMotionNode::TypeId());
  cannons = e->GetNodeList(CannonCtrlNode::TypeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Motion::Update(float dt) {
  auto enemy = aliens->head;
  auto ship=ships->head;
  auto cns= cannons->head;
  if (f::MainGame::Self()->IsOperational()) {
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
void Motion::ControlCannon(a::Node* node, float dt) {
  Cannon* gun = a::NodeFld<Cannon>(node, "cannon");
  Looper* lpr= a::NodeFld<Looper>(node, "looper");
  Ship* ship= a::NodeFld<Ship>(node, "ship");
  auto t= lpr->timer0;

  if (! gun->hasAmmo) {
    if (cx::TimerDone(t)) {
      ship->sprite->setSpriteFrame(ship->frame0);
      gun->hasAmmo=true;
      cx::UndoTimer(t);
      lpr->timer0=nullptr;
    }
  } else {
    if (f::MainGame::Get()->KeyPoll(c::EventKeyboard::KeyCode::KEY_SPACE)) {
      FireMissile(node,dt);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Motion::FireMissile(a::Node* node, float dt) {

  Cannon* gun= a::NodeFld<Cannon>(node, "cannon");
  Looper* lpr= a::NodeFld<Looper>(node, "looper");
  Ship* ship=a::NodeFld<Ship>(node, "ship");
  auto top= cx::GetTop(ship->sprite);
  auto cfg= f::XConfig::GetInstance();
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
  //sh.sfxPlay('ship-missile');
}

//////////////////////////////////////////////////////////////////////////
//
void Motion::ScanInput(a::Node* node, float dt) {
  Motion* m= a::NodeFld<Motion>(node, "motion");
  Ship* s= a::NodeFld<Ship>(node, "ship");

  if (f::MainGame::Get()->KeyPoll(
      c::EventKeyboard::KeyCode::KEY_RIGHT)) {
    m->right=true;
  }
  if (f::MainGame::Get()->KeyPoll(
      c::EventKeyboard::KeyCode::KEY_LEFT)) {
    m->left=true;
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Motion::ProcessAlienMotions(a::Node* node, float dt) {
  AlienSquad* sqad= a::NodeFld<AlienSquad>(node, "aliens");
  Looper* lpr = a::NodeFld<Looper>(node, "looper");
  auto g= f::MainGame::Get();

  if (ENP(lpr->timer0)) {
    lpr->timer0= nullptr;
    cx::CreateTimer(g,1);
  }

  if (ENP(lpr->timer1)) {
    lpr->timer1= nullptr;
    cx::CreateTimer(g,2);
  }
}




NS_END(invaders)


