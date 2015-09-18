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
#include "base/ccMacros.h"
#include "Aliens.h"
#include <math.h>
NS_ALIAS(cx, fusilli::ccsx)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
Aliens* Aliens::Create(Factory* f, c::Dictionary* d) {
  auto s = new Aliens();
  s->Set(f,d);
  return s;
}

//////////////////////////////////////////////////////////////////////////
//
Aliens::~Aliens() {
}

//////////////////////////////////////////////////////////////////////////
//
Aliens::Aliens()
  : aliens(nullptr) {
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::RemoveFromEngine(a::Engine* e) {
  aliens= nullptr;
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::AddToEngine(a::Engine* e) {
  aliens = e->GetNodeList(AlienMotionNode::TypeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Aliens::Update(float dt) {
  auto node=aliens.head;

  if (f::MainGame::Self()->IsOperational() &&
     NNP(node)) {
    ProcessMovement(node,dt);
    ProcessBombs(node,dt);
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::ProcessMovement(a::Node* node, float dt) {
  auto lpr = node->looper;
  auto sqad= node.aliens;
  auto tm= lpr->timer0;

  if (cx::TimerDone(tm)) {
    MaybeShuffleAliens(sqad);
    cx::UndoTimer(tm);
    lpr->timer0 =nullptr;
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::ProcessBombs(a::Node* node, float dt) {
  auto lpr = node->looper;
  auto sqad= node.aliens;
  auto tm= lpr->timer1;

  if (cx::TimerDone(tm)) {
    CheckBomb(sqad);
    cx::UndoTimer(tm);
    lpr->timer1= nullptr;
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::CheckBomb(f::ComObj* sqad) {
  f::XPool* p= sqad->pool;
  auto sz = p->Size();
  s::vector<int> rc;
  int n;

  for (n=0; n < sz; ++n) {
    if (p->Get(n)->status) {
      rc.push_back(n);
    }
  }

  if (rc.size() > 0) {
    n = floor( c::rand_0_1() * rc.size());
    auto pos= p->Get(n)->sprite->getPosition();
    DropBomb(pos.x, pos.y-4);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::DropBomb(float x, float y) {
  auto cfg = f::XCofig::GetInstance();
  auto bbs = cfg->GetPool("bombs");
  auto ent = bbs->Get();

  if (NNP(ent)) {
    factory->CreateBombs(25);
    ent = bbs->Get();
  }

  CCLOG("got one bomb from pool");
  ent->Inflate(x, y);
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::MaybeShuffleAliens(f::ComObj* sqad) {
  auto b = sqad->stepx > 0 ?
    FindMaxX(sqad) : FindMinX(sqad);
  bool ok;

  if (NNP(b) && b->status) {
    ok = TestDirX(b, sqad->stepx) ?
      DoShuffle(sqad) : DoForward(sqad);
    if (ok) {
      cx::SfxPlay("bugs-march");
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
bool Aliens::TestDirX(f::ComObj* b, int stepx) {
  auto wz= cx::VisRect();
  auto wb= cx::VisBox();
  auto sp= b->sprite;

  if (stepx > 0) {
    return cx::GetRight(sp) + stepx < (wb.right - (2/40 * wz.width));
  } else {
    return cx::GetLeft(sp) + stepx > (wb.left + (2/40 * wz.width));
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::ShuffleOneAlien(f::ComObj* a, int stepx) {
  auto pos= a->sprite->getPosition();
  a->sprite->setPosition(pos.x + stepx, pos.y);
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::ForwardOneAlien(f::ComObj* a, float delta) {
  auto pos= a->sprite->getPosition();
  auto wz= cx::VisRect();
  auto wb= cx::VisBox();
  a->sprite->setPosition(pos.x, pos.y - delta);
}

//////////////////////////////////////////////////////////////////////////
//
bool Aliens::DoShuffle(f::ComObj* sqad) {
  auto c= sqad->Elements();
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
bool Aliens::DoForward(f::ComObj* sqad) {
  auto delta= abs(sqad->stepx);
  auto found= false;
  auto c= sqad->Elements();

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
float Aliens::FindMinX(f::ComObj* sqad) {
  auto cur= (float) INT32_MAX;
  auto c= sqad->Elements();

  for (auto it= c.begin(); it != c.end(); ++it) {
    auto e= *it;
    if (e->status) {
      cur= min(cur, cx::GetLeft(a->sprite));
    }
  }
  return cur;
}

//////////////////////////////////////////////////////////////////////////
//
float Aliens::FindMaxX(f::ComObj* sqad) {
  auto cur= (float) INT32_MIN;
  auto c= sqad->Elements();

  for (auto it= c.begin(); it != c.end(); ++it) {
    auto e= *it;
    if (e->status) {
      cur= max(cur, cx::GetRight(a->sprite));
    }
  }
  return cur;
}


NS_END(invaders)

