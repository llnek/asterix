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
#include "Resolve.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(p2048)


//////////////////////////////////////////////////////////////////////////////
//
void Resolve::preamble() {
  _shared= _engine->getNodes("n/GVars")[0];
}

//////////////////////////////////////////////////////////////////////////////
//
bool Resolve::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::process(float dt) {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  if (maybeGameOver(ss)) {
    SENDMSG("/game/player/lose");
  }
  else if (ss->swiped) {
    postSwipe(ss);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::postSwipe(GVars *ss) {
  SENDMSG("/game/state/newnumber");
  ss->enabled=true;
  ss->swiped=false;
}

//////////////////////////////////////////////////////////////////////////////
//
bool Resolve::maybeGameOver(GVars *ss) {
  auto last=3;
  F__LOOP(it,ss->cardArr) {
    auto &arr= *it;
    for (auto i=0; i <= last; ++i) {
      if (arr->get(i)->getNumber() == 0) {
        return false;
      }
      if (i < last) {
        if (arr->get(i+1)->getNumber()==
            arr->get(i)->getNumber()) {
          return false;
        }
      }
    }
  }
  for (auto c=0; c < 4; ++c) {
    for (auto r=0; r < last; ++r) {
      if (ss->cardArr[r+1]->get(c)->getNumber()==
          ss->cardArr[r]->get(c)->getNumber()) {
         return false;
      }
    }
  }

  return true;
}

NS_END




