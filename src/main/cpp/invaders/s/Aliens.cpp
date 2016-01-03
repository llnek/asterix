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

#include "x2d/GameScene.h"
#include "base/ccMacros.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "core/ComObj.h"
#include "EFactory.h"
#include "Aliens.h"
#include <math.h>
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////
//
void Aliens::addToEngine(not_null<a::Engine*> e) {
  AlienMotionNode a;
  baddies = e->getNodeList(a.typeId());
  this->engine= (GEngine*) e.get();
}

//////////////////////////////////////////////////////////////////////////
//
bool Aliens::update(float dt) {
  auto node=baddies->head;
  if (MGMS()->isLive()) {
    processMovement(node, dt);
    processBombs(node, dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::processMovement(a::Node *node, float dt) {

  auto sqad= CC_GNF(AlienSquad, node, "aliens");
  auto lpr = CC_GNF(Looper, node, "looper");
  auto tm= lpr->timer0;

  if (cx::timerDone(tm)) {
    cx::undoTimer(tm);
    SNPTR(lpr->timer0)
    maybeShuffleAliens(sqad);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::processBombs(a::Node *node, float dt) {

  auto sqad= CC_GNF(AlienSquad, node, "aliens");
  auto lpr = CC_GNF(Looper, node, "looper");
  auto tm= lpr->timer1;

  if (cx::timerDone(tm)) {
    cx::undoTimer(tm);
    SNPTR(lpr->timer1)
    checkBomb(sqad);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::checkBomb(AlienSquad *sqad) {
  auto c= sqad->list();
  auto p= sqad->aliens;
  int sz = p->size();
  s_vec<int> rc;
  int pos=0;

  F__LOOP(it, c) {
    auto &a= *it;
    if (a->status) { rc.push_back(pos); }
    ++pos;
  }

  sz= rc.size();
  if (sz > 0) {
    pos = sz == 1 ? 0 : cx::randInt(sz);
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
    engine->reifyBombs(25);
    ent = bbs->get();
  }

  ent->inflate(x, y);
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::maybeShuffleAliens(AlienSquad *sqad) {
  auto b = sqad->stepx > 0 ? findMaxX(sqad) : findMinX(sqad);
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
bool Aliens::testDirX(f::ComObj *b, int stepx) {
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
void Aliens::shuffleOneAlien(f::ComObj *a, int stepx) {
  auto pos= a->sprite->getPosition();
  a->sprite->setPosition(pos.x + stepx, pos.y);
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::forwardOneAlien(f::ComObj *a, float delta) {
  auto pos= a->sprite->getPosition();
  auto wz= cx::visRect();
  auto wb= cx::visBox();
  a->sprite->setPosition(pos.x, pos.y - delta);
}

//////////////////////////////////////////////////////////////////////////
//
bool Aliens::doShuffle(AlienSquad *sqad) {
  auto c= sqad->list();
  auto found=false;

  F__LOOP(it, c) {
    auto &e= *it;
    if (e->status) {
      shuffleOneAlien(e, sqad->stepx);
      found=true;
    }
  }
  return found;
}

//////////////////////////////////////////////////////////////////////////
//
bool Aliens::doForward(AlienSquad* sqad) {
  auto delta= abs(sqad->stepx);
  auto c= sqad->list();
  auto found= false;

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
f::ComObj* Aliens::findMinX(AlienSquad* sqad) {
  auto cur= (float) INT32_MAX;
  auto c= sqad->list();
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
f::ComObj* Aliens::findMaxX(AlienSquad* sqad) {
  auto cur= (float) INT32_MIN;
  auto c= sqad->list();
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

