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

#include "Aliens.h"

//////////////////////////////////////////////////////////////////////////
//
Aliens::Aliens(options)
  : state(options) {
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::RemoveFromEngine(Engine* e) {
  aliens= nullptr;
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::AddToEngine(Engine* e) {
  aliens = e->GetNodeList(AlienMotionNode::TypeId());
}

//////////////////////////////////////////////////////////////////////////
//
float Aliens::Update(float dt) {
  auto node=aliens.head;

  if (MainGame::Get()->IsOperational() &&
     NNP(node)) {
    ProcessMovement(node,dt);
    ProcessBombs(node,dt);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::ProcessMovement(Node* node, float dt) {
  auto lpr = node->looper;
  auto sqad= node.aliens;
  auto tm= lpr->timers[0];

  if (CCSX::TimerDone(tm)) {
    MaybeShuffleAliens(sqad);
    lpr->timers[0]=CCSX::UndoTimer(tm);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::ProcessBombs(Node* node, float dt) {
  auto lpr = node->looper;
  auto sqad= node.aliens;
  auto tm= lpr->timers[1];

  if (CCSX::TimerDone(tm)) {
    CheckBomb(sqad);
    lpr->timers[1]=CCSX::UndoTimer(tm);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::CheckBomb(ComObj* sqad) {
  XPool* p= sqad->aliens->pool;
  auto sz = p->Size();
  vector<int> rc;
  int n;

  for (n=0; n < sz; ++n) {
    if (p->Get(n)->status) {
      rc.push_back(n);
    }
  }
  if (rc.size() > 0) {
    n = rc.size() == 1 ? 0 : sjs.rand(rc.size());
    auto pos= p->Get(n)->sprite->getPosition();
    DropBomb(pos.x, pos.y-4);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::DropBomb(float x, float y) {
  auto cfg = XCofig::GetInstance();
  auto bbs = cfg->GetPool("bombs");
  auto ent = bbs->Get();

  if (NNP(ent)) {
    factory->CreateBombs(25);
    ent = bbs->Get();
  }

  //sjs.loggr.debug('got one bomb from pool');
  ent->InflateAt(x, y);
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::MaybeShuffleAliens(sqad) {
  auto b = sqad->stepx > 0 ?
    FindMaxX(sqad) : FindMinX(sqad);
  bool ok;

  if (NNP(b) && b->status) {
    ok = TestDirX(b, sqad->stepx) ?
      DoShuffle(sqad) : DoForward(sqad);
    if (ok) {
      sh.sfxPlay('bugs-march');
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
bool Aliens::TestDirX(ComObj* b, int stepx) {
  auto wz= CCSX::VisRect();
  auto wb= CCSX::VisBox();
  auto sp= b->sprite;
  if (stepx > 0) {
    return CCSX::GetRight(sp) + stepx < (wb.right - (2/40 * wz.width));
  } else {
    return CCSX::GetLeft(sp) + stepx > (wb.left + (2/40 * wz.width));
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::ShuffleOneAlien(ComObj* a, int stepx) {
  auto pos= a->sprite->getPosition();
  a->sprite->setPosition(pos.x + stepx, pos.y);
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::ForwardOneAlien(ComObj* a, float delta) {
  auto pos= a->sprite->getPosition();
  auto wz= CCSX::VisRect();
  auto wb= CCSX::VisBox();
  a->sprite->setPosition(pos.x, pos.y - delta);
}

//////////////////////////////////////////////////////////////////////////
//
bool Aliens::DoShuffle(sqad) {
  auto c= sqad->aliens->pool->elements();
  auto found=false;
  for (auto it= c.begin(); it != c.end(); ++it) {
    auto e= *it;
    if (e->status) {
      ShuffleOneAlien(e,sqad->stepx);
      found=true;
    }
  }
  return found;
}

//////////////////////////////////////////////////////////////////////////
//
bool Aliens::DoForward(ComObj* sqad) {
  auto delta= abs(sqad->stepx);
  auto found= false;
  auto c= sqad->aliens->pool->elements();
  for (auto it= c.begin(); it != c.end(); ++it) {
    auto e= *it;
    if (e->status) {
      ForwardOneAlien(e, delta);
      found=true;
    }
  }
  sqad->stepx = - sqad->stepx;
  return found;
}

//////////////////////////////////////////////////////////////////////////
//
float Aliens::FindMinX(ComObj* sqad) {
  auto c= sqad->aliens->pool->elements();
  auto cur= (float) INT32_MAX;
  for (auto it= c.begin(); it != c.end(); ++it) {
    auto e= *it;
    if (e->status) {
      cur= min(cur, CCSX::GetLeft(a->sprite));
    }
  }
  return cur;
}

//////////////////////////////////////////////////////////////////////////
//
float Aliens::FindMaxX(ComObj* sqad) {
  auto c= sqad->aliens->pool->elements();
  auto cur= (float) INT32_MIN;
  for (auto it= c.begin(); it != c.end(); ++it) {
    auto e= *it;
    if (e->status) {
      cur= max(cur, CCSX::GetRight(a->sprite));
    }
  }
  return cur;
}


NS_END(invaders)

