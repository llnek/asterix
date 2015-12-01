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
#include "x2d/GameScene.h"
#include "Aliens.h"
#include <math.h>
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////
//
Aliens::Aliens(not_null<EFactory*> f, not_null<c::Dictionary*> d)
  : f::BaseSystem<EFactory>(f,d) {
  SNPTR(baddies)
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::addToEngine(not_null<a::Engine*> e) {
  //CCLOG("adding system: Aliens");
  AlienMotionNode a;
  baddies = e->getNodeList(a.TypeId());
  //CCLOG("added system: Aliens");
}

//////////////////////////////////////////////////////////////////////////
//
bool Aliens::onUpdate(float dt) {
  auto node=baddies->head;
  if (MGMS()->isLive() && NNP(node)) {
    processMovement(node,dt);
    processBombs(node,dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::processMovement(not_null<a::Node*> node, float dt) {

  auto sqad= CC_GNF(AlienSquad, node, "aliens");
  auto lpr = CC_GNF(Looper, node, "looper");
  auto tm= lpr->timer0;

  if (cx::timerDone(tm)) {
    maybeShuffleAliens(sqad);
    cx::undoTimer(tm);
    SNPTR(lpr->timer0)
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::processBombs(not_null<a::Node*> node, float dt) {

  auto sqad= CC_GNF(AlienSquad, node, "aliens");
  auto lpr = CC_GNF(Looper, node, "looper");
  auto tm= lpr->timer1;

  if (cx::timerDone(tm)) {
    checkBomb(sqad);
    cx::undoTimer(tm);
    SNPTR(lpr->timer1)
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::checkBomb(not_null<AlienSquad*> sqad) {
  auto p= sqad->aliens;
  int sz = p->size();
  s::vector<int> rc;

  auto c = sqad->elements();
  int pos=0;
  F__LOOP(it, c) {
    auto a=*it;
    if (a->status) { rc.push_back(pos); }
    ++pos;
  }

  sz= rc.size();
  if (sz > 0) {
    pos = sz == 1 ? 0 : c::random(0, sz);
    auto n=rc[pos];
    auto v= p->getAt(n)->sprite->getPosition();
    dropBomb(v.x, v.y-4);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::dropBomb(float x, float y) {
  auto bbs = MGMS()->getPool("bombs");
  auto ent = bbs->get();

  if (NNP(ent)) {
    factory->reifyBombs(25);
    ent = bbs->get();
  }

  ent->inflate(x, y);
  //CCLOG("got one bomb from pool");
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::maybeShuffleAliens(not_null<AlienSquad*> sqad) {
  auto b = sqad->stepx > 0 ?
    FindMaxX(sqad) : FindMinX(sqad);
  bool ok;

  if (NNP(b) && b->status) {
    ok = testDirX(b, sqad->stepx) ? doShuffle(sqad) : doForward(sqad);
    if (ok) {
      cx::sfxPlay("bugs-march");
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
bool Aliens::testDirX(not_null<f::ComObj*> b, int stepx) {
  auto wz= cx::visRect();
  auto wb= cx::visBox();
  auto sp= b->sprite;

  if (stepx > 0) {
    return cx::getRight(sp) + stepx < (wb.right - (2/40.0f * wz.size.width));
  } else {
    return cx::getLeft(sp) + stepx > (wb.left + (2/40.0f * wz.size.width));
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::shuffleOneAlien(not_null<f::ComObj*> a, int stepx) {
  auto pos= a->sprite->getPosition();
  a->sprite->setPosition(pos.x + stepx, pos.y);
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::forwardOneAlien(not_null<f::ComObj*> a, float delta) {
  auto pos= a->sprite->getPosition();
  auto wz= cx::visRect();
  auto wb= cx::visBox();
  a->sprite->setPosition(pos.x, pos.y - delta);
}

//////////////////////////////////////////////////////////////////////////
//
bool Aliens::doShuffle(not_null<AlienSquad*> sqad) {
  auto c= sqad->elements();
  auto found=false;

  F__LOOP(it, c) {
    auto e= *it;
    if (e->status) {
      shuffleOneAlien(e, sqad->stepx);
      found=true;
    }
  }
  return found;
}

//////////////////////////////////////////////////////////////////////////
//
bool Aliens::doForward(not_null<AlienSquad*> sqad) {
  auto delta= abs(sqad->stepx);
  auto found= false;
  auto c= sqad->elements();

  F__LOOP(it, c) {
    auto e= *it;
    if (e->status) {
      forwardOneAlien(e, delta);
      found=true;
    }
  }
  sqad->stepx = - sqad->stepx;
  return found;
}

//////////////////////////////////////////////////////////////////////////
//
f::ComObj* Aliens::findMinX(not_null<AlienSquad*> sqad) {
  auto cur= (float) INT32_MAX;
  auto c= sqad->elements();
  f::ComObj* rc= nullptr;
  float v;

  F__LOOP(it, c) {
    auto e= *it;
    if (e->status) {
      v= cx::getLeft(e->sprite);
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
f::ComObj* Aliens::findMaxX(not_null<AlienSquad*> sqad) {
  auto cur= (float) INT32_MIN;
  auto c= sqad->elements();
  f::ComObj* rc= nullptr;
  float v;

  F__LOOP(it, c) {
    auto e= *it;
    if (e->status) {
      v= cx::getRight(e->sprite);
      if (v > cur) {
        cur = v;
        rc=e;
      }
    }
  }
  return rc;
}


NS_END(invaders)

