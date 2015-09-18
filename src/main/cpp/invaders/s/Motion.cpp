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
#include "2d/MainGame.h"
#include "Motion.h"
NS_ALIAS(cx, fusilli::ccsx)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
Motions* Motions::Create(Factory* f, c::Dictionary* d) {
  auto s = new Motions();
  s->Set(f,d);
  return s;
}

//////////////////////////////////////////////////////////////////////////
//
Motions::~Motions() {
}

//////////////////////////////////////////////////////////////////////////
//
Motions::Motions()
  : aliens(nullptr)
  , ships(nullptr)
  , cannons(nullptr) {
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::RemoveFromEngine(a::Engine* e) {
  aliens = nullptr;
  ships = nullptr;
  cannons= nullptr;
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::AddToEngine(a::Engine* e) {
  aliens = e->GetNodeList(AlienMotionNode::TypeId());
  ships = e->GetNodeList(ShipMotionNode::TypeId());
  cannons = e->GetNodeList(CannonCtrlNode::TypeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Motions::Update(float dt) {
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
}

//////////////////////////////////////////////////////////////////////////
//
void Motion::ControlCannon(a::Node* node, float dt) {
  auto gun = node->cannon;
  auto lpr= node->looper;
  auto ship=node->ship;
  auto t= lpr->timer0;

  if (! gun->hasAmmo) {
    if (cx::TimerDone(t)) {
      ship->sprite->setSpriteFrame(ship->frame0);
      gun->hasAmmo=true;
      cx::UndoTimer(t);
      lpr->timer0=nullptr;
    }
  } else {
    if (f::MainGame::Get()->KeyPoll(cc.KEY.space)) {
      FireMissile(node,dt);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Motion::FireMissile(a::Node* node, float dt) {

  auto top= cx::GetTop(node->ship->sprite);
  auto cfg= f::XConfig::GetInstance();
  auto p= cfg->GetPool("missiles");
  auto ship=node->ship;
  auto pos= ship->Pos();
  auto lpr= node->looper;
  auto gun= node->cannon;
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
  auto m= node->motion;
  auto s= node->ship,

  if (f::MainGame::Get()->KeyPoll(cc.KEY.right)) {
    m->right=true;
  }
  if (f::MainGame::Get()->KeyPoll(cc.KEY.left)) {
    m->left=true;
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Motion::ProcessAlienMotions(a::Node* node, float dt) {
  auto g= f::MainGame::Get();
  auto lpr = node->looper,
  auto sqad= node->aliens;

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


