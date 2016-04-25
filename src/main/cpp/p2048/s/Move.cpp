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

//////////////////////////////////////////////////////////////////////////////

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Move.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(p2048)


//////////////////////////////////////////////////////////////////////////////
//
void Move::preamble() {
  _player= _engine->getNodes("f/CGesture")[0];
  _shared= _engine->getNodes("n/GVars")[0];
}

//////////////////////////////////////////////////////////////////////////////
//
bool Move::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::process(float dt) {
  auto g=CC_GEC(f::CGesture,_player,"f/CGesture");
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  onKeys(g, dt);
  if (g->right) { swipeRight(ss); }
  if (g->left) { swipeLeft(ss); }
  if (g->up) { swipeUp(ss); }
  if (g->down) { swipeDown(ss); }
  postSwipe(ss);
  if (!ss->enabled) {
     ss->enabled=true;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::postSwipe(GVars *ss) {
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::swipeDown(GVars *ss) {
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::swipeUp(GVars *ss) {
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::swipeRight(GVars *ss) {
  for (auto r= 0; r < ss->cardArr.size(); ++r) {
    swipeRight( ss->cardArr[r] );
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::swipeRight(CardArr *arr) {
  int tmp[4] = { 1,1,1,1 };
  auto last= arr->size() - 1;
  for (auto i=last; i >= 0; --i) {
    auto c= arr->get(i);
    auto v= c->getNumber();
    if (v ==0) { continue; }
    for (auto n=i+1; n <= last; ++n) {
      auto c2=arr->get(n);
      auto v2=c2->getNumber();
      if (v2 > 0) {
        c->setNumber(0);
        if (v==v2 && tmp[n]>0) {
          c2->setNumber(v+v2);
          tmp[n]=0;
        } else {
          arr->get(n-1)->setNumber(v);
        }
        break;
      } else if (n== last){
        c->setNumber(0);
        c2->setNumber(v);
        break;
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::swipeLeft(GVars *ss) {
  for (auto r= 0; r < ss->cardArr.size(); ++r) {
    swipeLeft( ss->cardArr[r] );
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::swipeLeft(CardArr *arr) {
  int tmp[4] = { 1,1,1,1 };
  auto last= arr->size() - 1;
  for (auto i=0; i <= last; ++i) {
    auto c= arr->get(i);
    auto v= c->getNumber();
    if (v ==0) { continue; }
    for (auto n=i-1; n >= 0; --n) {
      auto c2=arr->get(n);
      auto v2=c2->getNumber();
      if (v2 > 0) {
        c->setNumber(0);
        if (v==v2 && tmp[n]>0) {
          c2->setNumber(v+v2);
          tmp[n]=0;
        } else {
          arr->get(n+1)->setNumber(v);
        }
        break;
      } else if (n== 0){
        c->setNumber(0);
        c2->setNumber(v);
        break;
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::onKeys(f::CGesture *g, float dt) {
}


NS_END




