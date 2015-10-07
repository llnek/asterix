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

#include "base/ccMacros.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "core/ComObj.h"
#include "x2d/MainGame.h"
#include "Aliens.h"
#include <math.h>
NS_ALIAS(cx, fusii::ccsx)
NS_ALIAS(f, fusii)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
Aliens::Aliens(not_null<Factory*> f, not_null<c::Dictionary*> d) {
  SNPTR(baddies)
  Set(f,d);
}

//////////////////////////////////////////////////////////////////////////
//
Aliens::~Aliens() {
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::RemoveFromEngine(not_null<a::Engine*> e) {
  SNPTR(baddies)
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::AddToEngine(not_null<a::Engine*> e) {
  CCLOG("adding system: Aliens");
  AlienMotionNode a;
  baddies = e->GetNodeList(a.TypeId());
  CCLOG("added system: Aliens");
}

//////////////////////////////////////////////////////////////////////////
//
bool Aliens::OnUpdate(float dt) {
  auto node=baddies->head;

  if (MGMS()->IsRunning() && NNP(node)) {
    ProcessMovement(node,dt);
    ProcessBombs(node,dt);
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::ProcessMovement(not_null<a::Node*> node, float dt) {

  auto sqad= a::NodeFld<AlienSquad>(node, "aliens");
  auto lpr = a::NodeFld<Looper>(node, "looper");
  auto tm= lpr->timer0;

  if (cx::TimerDone(tm)) {
    MaybeShuffleAliens(sqad);
    cx::UndoTimer(tm);
    SNPTR(lpr->timer0)
  }

}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::ProcessBombs(not_null<a::Node*> node, float dt) {

  auto sqad= a::NodeFld<AlienSquad>(node, "aliens");
  auto lpr = a::NodeFld<Looper>(node, "looper");
  auto tm= lpr->timer1;

  if (cx::TimerDone(tm)) {
    CheckBomb(sqad);
    cx::UndoTimer(tm);
    SNPTR(lpr->timer1)
  }

}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::CheckBomb(not_null<AlienSquad*> sqad) {
  auto p= sqad->aliens;
  auto sz = p->Size();
  s::vector<int> rc;

  auto c = sqad->Elements();
  int pos=0;
  for (auto it = c.begin(); it != c.end(); ++it) {
    auto a=*it;
    if (a->status) { rc.push_back(pos); }
    ++pos;
  }

  if (rc.size() > 0) {
    pos = floor( c::rand_0_1() * rc.size());
    int n=rc[pos];
    auto v= p->GetAt(n)->sprite->getPosition();
    DropBomb(v.x, v.y-4);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::DropBomb(float x, float y) {
  auto bbs = XCFGS()->GetPool("bombs");
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
void Aliens::MaybeShuffleAliens(not_null<AlienSquad*> sqad) {
  auto b = sqad->stepx > 0 ?
    FindMaxX(sqad) : FindMinX(sqad);
  bool ok;

  if (NNP(b) && b->status) {
    ok = TestDirX(b, sqad->stepx) ? DoShuffle(sqad) : DoForward(sqad);
    if (ok) {
      cx::SfxPlay("bugs-march");
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
bool Aliens::TestDirX(not_null<f::ComObj*> b, int stepx) {
  auto wz= cx::VisRect();
  auto wb= cx::VisBox();
  auto sp= b->sprite;

  if (stepx > 0) {
    return cx::GetRight(sp) + stepx < (wb.right - (2/40 * wz.size.width));
  } else {
    return cx::GetLeft(sp) + stepx > (wb.left + (2/40 * wz.size.width));
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::ShuffleOneAlien(not_null<f::ComObj*> a, int stepx) {
  auto pos= a->sprite->getPosition();
  a->sprite->setPosition(pos.x + stepx, pos.y);
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::ForwardOneAlien(not_null<f::ComObj*> a, float delta) {
  auto pos= a->sprite->getPosition();
  auto wz= cx::VisRect();
  auto wb= cx::VisBox();
  a->sprite->setPosition(pos.x, pos.y - delta);
}

//////////////////////////////////////////////////////////////////////////
//
bool Aliens::DoShuffle(not_null<AlienSquad*> sqad) {
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
bool Aliens::DoForward(not_null<AlienSquad*> sqad) {
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
f::ComObj* Aliens::FindMinX(not_null<AlienSquad*> sqad) {
  auto cur= (float) INT32_MAX;
  auto c= sqad->Elements();
  f::ComObj* rc= nullptr;
  float v;

  for (auto it= c.begin(); it != c.end(); ++it) {
    auto e= *it;
    if (e->status) {
      v= cx::GetLeft(e->sprite);
      if (v < cur) {
       cur=v;
       rc=e;
      }
    }
  }
  return rc;
}

//////////////////////////////////////////////////////////////////////////
//
f::ComObj* Aliens::FindMaxX(not_null<AlienSquad*> sqad) {
  auto cur= (float) INT32_MIN;
  auto c= sqad->Elements();
  f::ComObj* rc= nullptr;
  float v;

  for (auto it= c.begin(); it != c.end(); ++it) {
    auto e= *it;
    if (e->status) {
      v= cx::GetRight(e->sprite);
      if (v > cur) {
        cur = v;
        rc=e;
      }
    }
  }
  return rc;
}


NS_END(invaders)

