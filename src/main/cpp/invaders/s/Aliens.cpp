// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

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
  lpr->tms.push_back(f::DLTimer());
  lpr->tms.push_back(f::DLTimer());
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
  auto &tm= lpr->tms[0];

  if (cx::timerDone(tm.timer)) {
    cx::undoTimer(tm.timer);
    tm.timer= CC_NIL;
    maybeShuffleAliens(sqad);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::processBombs(float dt) {

  auto sqad= CC_GEC(AlienSquad, _baddies, "n/AlienSquad");
  auto lpr = CC_GEC(f::Loopers, _baddies, "f/Loopers");
  auto &tm= lpr->tms[1];

  if (cx::timerDone(tm.timer)) {
    cx::undoTimer(tm.timer);
    tm.timer= CC_NIL;
    checkBomb(sqad);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::checkBomb(AlienSquad *sqad) {
  auto &c= sqad->aliens->ls();
  auto sz= c.size();
  s_vec<int> rc;
  int idx=0;

  F__LOOP(it, c) {
    auto &a= *it;
    if (a->status()) { rc.push_back(idx); }
    ++idx;
  }

  // randomly pick an alien to drop bomb
  sz= rc.size();
  if (sz > 0) {
    idx = sz == 1 ? 0 : cx::randInt(sz);
    auto n=rc[idx];
    auto e= sqad->aliens->getAt(n);
    auto s= CC_GEC(f::CPixie, e, "f/CPixie");
    auto v= s->pos();
    dropBomb(v.x, v.y-4);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::dropBomb(float x, float y) {
  auto bbs = MGMS()->getPool("Bombs");
  auto e = bbs->take(true);
  if (e) {
    auto h= CC_GEC(f::CHealth,e, "f/CHealth");
    auto s= CC_GEC(f::CPixie,e, "f/CPixie");
    cx::resurrect((ecs::Node*)e, x,y);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::maybeShuffleAliens(AlienSquad *sqad) {
  auto b = sqad->stepx > 0 ? findMaxX(sqad) : findMinX(sqad);
  bool ok;

  if (N_NIL(b) && b->status()) {
    ok = testDirX(b, sqad->stepx) ? doShuffle(sqad) : doForward(sqad);
    if (ok) {
      cx::sfxPlay("bugs-march");
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
bool Aliens::testDirX(ecs::Node *b, int stepx) {
  auto sp= CC_GEC(f::CPixie,b,"f/CPixie");
  auto wz= cx::visSize();
  auto wb= cx::visBox();
  auto delta= 2/40.0 * wz.width;

  if (stepx > 0) {
    return cx::getRight(sp) + stepx < wb.right - delta;
  } else {
    return cx::getLeft(sp) + stepx > wb.left + delta;
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::shuffleOneAlien(ecs::Node *a, int stepx) {
  auto sp= CC_GEC(f::CPixie,a,"f/CPixie");
  auto pos= sp->getPosition();
  CC_POS2(sp, pos.x + stepx, pos.y);
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::forwardOneAlien(ecs::Node *a, float delta) {
  auto sp= CC_GEC(f::CPixie,a,"f/CPixie");
  auto pos= sp->getPosition();
  CC_POS2(sp, pos.x, pos.y - delta);
}

//////////////////////////////////////////////////////////////////////////
//
bool Aliens::doShuffle(AlienSquad *sqad) {
  auto &c= sqad->aliens->ls();
  auto found=false;

  F__LOOP(it, c) {
    auto &e=  *it;
    if (e->status()) {
        shuffleOneAlien((ecs::Node*)e, sqad->stepx);
      found=true;
    }
  }

  return found;
}

//////////////////////////////////////////////////////////////////////////
//
bool Aliens::doForward(AlienSquad *sqad) {
  auto delta= abs(sqad->stepx);
  auto &c= sqad->aliens->ls();
  auto found= false;

  F__LOOP(it, c) {
    auto &e=  *it;
    if (e->status()) {
        forwardOneAlien((ecs::Node*)e, delta);
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
  auto &co= sqad->aliens->ls();
  ecs::Node *rc= CC_NIL;
  float v;

  F__LOOP(it, co) {
    auto &e= *it;
    auto c=CC_GEC(f::CPixie,e,"f/CPixie");
    if (e->status()) {
      v= cx::getLeft(c);
      if (v < cur) {
        cur=v;
        rc= (ecs::Node*)e;
      }
    }
  }

  return rc;
}

//////////////////////////////////////////////////////////////////////////
//
ecs::Node* Aliens::findMaxX(AlienSquad *sqad) {
  auto cur= (float) INT32_MIN;
  auto &co= sqad->aliens->ls();
  ecs::Node* rc= CC_NIL;
  float v;

  F__LOOP(it, co) {
    auto &e= *it;
    auto c= CC_GEC(f::CPixie,e,"f/CPixie");
    if (e->status()) {
      v= cx::getRight(c);
      if (v > cur) {
        cur = v;
        rc= (ecs::Node*) e;
      }
    }
  }

  return rc;
}



NS_END

