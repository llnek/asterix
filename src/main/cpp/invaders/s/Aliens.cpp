// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Ken Leung. All rights reserved.

#include "x2d/GameScene.h"
#include "base/ccMacros.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Aliens.h"
#include <math.h>
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////
//
void Aliens::preamble() {
  _baddies = _engine->getNodes("n/AlienMotion")[0];
}

//////////////////////////////////////////////////////////////////////////
//
bool Aliens::update(float dt) {
  if (MGMS()->isLive()) {
    processMovement( dt);
    processBombs( dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::processMovement(float dt) {

  auto sqad= CC_GEC(AlienSquad, _baddies, "n/AlienSquad");
  auto lpr = CC_GEC(Looper, _baddies, "n/Looper");
  auto tm= lpr->timer0;

  if (cx::timerDone(tm)) {
    cx::undoTimer(tm);
    SNPTR(lpr->timer0)
    maybeShuffleAliens(sqad);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::processBombs(float dt) {

  auto sqad= CC_GEC(AlienSquad, _baddies, "n/AlienSquad");
  auto lpr = CC_GEC(Looper, _baddies, "n/Looper");
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
  auto sz= sqad->aliens-size();
  auto c= sqad->ls();
  s_vec<int> rc;
  int idx=0;

  F__LOOP(it, c) {
    auto &a= *it;
    if (a->status()) { rc.push_back(idx); }
    ++idx;
  }

  sz= rc.size();
  if (sz > 0) {
    idx = sz == 1 ? 0 : cx::randInt(sz);
    auto n=rc[idx];
    auto v= sqad->aliens->getAt(n)->pos();
    dropBomb(v.x, v.y-4);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::dropBomb(float x, float y) {
  auto bbs = MGMS()->getPool("Bombs");
  auto ent = bbs->get();

  if (NNP(ent)) {
    SCAST(GEngine*, _engine)->reifyBombs(25);
    ent = bbs->get();
  }

  ent->inflate(x, y);
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::maybeShuffleAliens(AlienSquad *sqad) {
  auto b = sqad->stepx > 0 ? findMaxX(sqad) : findMinX(sqad);
  bool ok;

  if (NNP(b) && b->status()) {
    ok = testDirX(b, sqad->stepx) ? doShuffle(sqad) : doForward(sqad);
    if (ok) {
      cx::sfxPlay("bugs-march");
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
bool Aliens::testDirX(ecs::Node *b, int stepx) {
  auto sp= CC_GEC(f::CDraw,b,"f/CDraw")->node;
  auto wz= cx::visRect();
  auto wb= cx::visBox();
  auto delta= 2/40.0 * wz.size.width;

  if (stepx > 0) {
    return cx::getRight(sp) + stepx < wb.right - delta;
  } else {
    return cx::getLeft(sp) + stepx > wb.left + delta;
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::shuffleOneAlien(ecs::Node *a, int stepx) {
  auto sp= CC_GEC(f::CDraw,a,"f/CDraw")->node;
  auto pos= sp->getPosition();
  sp->setPosition(pos.x + stepx, pos.y);
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::forwardOneAlien(ecs::Node *a, float delta) {
  auto sp= CC_GEC(f::CDraw,a,"f/CDraw")->node;
  auto pos= sp->getPosition();
  sp->setPosition(pos.x, pos.y - delta);
}

//////////////////////////////////////////////////////////////////////////
//
bool Aliens::doShuffle(AlienSquad *sqad) {
  auto c= sqad->ls();
  auto found=false;

  F__LOOP(it, c) {
    auto &e= *it;
    if (e->status()) {
      shuffleOneAlien(e, sqad->stepx);
      found=true;
    }
  }
  return found;
}

//////////////////////////////////////////////////////////////////////////
//
bool Aliens::doForward(AlienSquad *sqad) {
  auto delta= abs(sqad->stepx);
  auto c= sqad->ls();
  auto found= false;

  F__LOOP(it, c) {
    auto e= *it;
    if (e->status()) {
      forwardOneAlien(e, delta);
      found=true;
    }
  }
  sqad->stepx = - sqad->stepx;
  return found;
}

//////////////////////////////////////////////////////////////////////////
//
ecs::Node* Aliens::findMinX(AlienSquad *sqad) {
  auto cur= (float) INT32_MAX;
  auto c= sqad->ls();
  ecs::Node *rc= nullptr;
  float v;

  F__LOOP(it, c) {
    auto e= *it;
    auto sp=CC_GEC(f::CDraw,e,"f/CDraw");
    if (e->status()) {
      v= cx::getLeft(sp->node);
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
ecs::Node* Aliens::findMaxX(AlienSquad *sqad) {
  auto cur= (float) INT32_MIN;
  auto c= sqad->ls();
  ecs::Node* rc= nullptr;
  float v;

  F__LOOP(it, c) {
    auto e= *it;
    auto sp= CC_GEC(f::CDraw,e,"f/CDraw");
    if (e->status()) {
      v= cx::getRight(sp->node);
      if (v > cur) {
        cur = v;
        rc=e;
      }
    }
  }

  return rc;
}


NS_END

