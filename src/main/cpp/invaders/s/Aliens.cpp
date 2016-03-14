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
  _baddies = _engine->getNodes("n/AlienSquad")[0];
  auto lpr = CC_GEC(f::Loopers, _baddies, "f/Loopers");
  // 2 timers, 1 for moves, 1 for bombs
  lpr->timers.push_back(CC_NIL);
  lpr->timers.push_back(CC_NIL);
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
  auto lpr = CC_GEC(f::Loopers, _baddies, "f/Loopers");
  auto tm= lpr->timers[0];

  if (cx::timerDone(tm)) {
    lpr->timers[0]= CC_NIL;
    cx::undoTimer(tm);
    maybeShuffleAliens(sqad);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::processBombs(float dt) {

  auto sqad= CC_GEC(AlienSquad, _baddies, "n/AlienSquad");
  auto lpr = CC_GEC(f::Loopers, _baddies, "f/Loopers");
  auto tm= lpr->timers[1];

  if (cx::timerDone(tm)) {
    lpr->timers[1]= CC_NIL;
    cx::undoTimer(tm);
    checkBomb(sqad);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::checkBomb(AlienSquad *sqad) {
  auto c= sqad->aliens->ls();
  auto sz= c.size();
  s_vec<int> rc;
  int idx=0;

  F__LOOP(it, c) {
    auto a= *it;
    if (a->status()) { rc.push_back(idx); }
    ++idx;
  }

  // randomly pick an alien to drop bomb
  sz= rc.size();
  if (sz > 0) {
    idx = sz == 1 ? 0 : cx::randInt(sz);
    auto n=rc[idx];
    auto e= (ecs::Node*) sqad->aliens->getAt(n);
    auto s= CC_GEC(f::CPixie, e, "f/CPixie");
    auto v= s->pos();
    dropBomb(v.x, v.y-4);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::dropBomb(float x, float y) {
  auto bbs = MGMS()->getPool("Bombs");
    auto e = (ecs::Node*) bbs->take(true);
    if (e) {
      auto h= CC_GEC(f::CHealth,e, "f/CHealth");
      auto s= CC_GEC(f::CPixie,e, "f/CPixie");
      s->inflate(x,y);
      h->reset();
    }
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
  auto cd= CC_GEC(f::CPixie,b,"f/CPixie");
  auto wz= cx::visRect();
  auto wb= cx::visBox();
  auto sp= cd->node;
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
  auto cd= CC_GEC(f::CPixie,a,"f/CPixie");
  auto sp= cd->node;
  auto pos= sp->getPosition();
  sp->setPosition(pos.x + stepx, pos.y);
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::forwardOneAlien(ecs::Node *a, float delta) {
  auto cd= CC_GEC(f::CPixie,a,"f/CPixie");
  auto sp= cd->node;
  auto pos= sp->getPosition();
  sp->setPosition(pos.x, pos.y - delta);
}

//////////////////////////////////////////////////////////////////////////
//
bool Aliens::doShuffle(AlienSquad *sqad) {
  auto c= sqad->aliens->ls();
  auto found=false;

  F__LOOP(it, c) {
    auto e= (ecs::Node*) *it;
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
  auto c= sqad->aliens->ls();
  auto found= false;

  F__LOOP(it, c) {
    auto e= (ecs::Node*) *it;
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
  auto co= sqad->aliens->ls();
  ecs::Node *rc= nullptr;
  float v;

  F__LOOP(it, co) {
    auto e= (ecs::Node*) *it;
    auto c=CC_GEC(f::CPixie,e,"f/CPixie");
    if (e->status()) {
      v= cx::getLeft(c->node);
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
  auto co= sqad->aliens->ls();
  ecs::Node* rc= nullptr;
  float v;

  F__LOOP(it, co) {
    auto e= (ecs::Node*) *it;
    auto c= CC_GEC(f::CPixie,e,"f/CPixie");
    if (e->status()) {
      v= cx::getRight(c->node);
      if (v > cur) {
        cur = v;
        rc=e;
      }
    }
  }

  return rc;
}


NS_END

