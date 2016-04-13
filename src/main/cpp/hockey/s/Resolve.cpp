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

//////////////////////////////////////////////////////////////////////////////

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Resolve.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(hockey)

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::preamble() {
  _shared= _engine->getNodes("n/GVars")[0];
  _puck= _engine->getNodes("f/CAutoma")[0];
  _engine->getNodes("f/CGesture",_mallets);
}

//////////////////////////////////////////////////////////////////////////////
//
bool Resolve::update(float dt) {
  if (MGMS()->isLive()) {
    process();
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::process() {
  auto pv = CC_GEC(f::CMove,_puck,"f/CMove");
  auto ss = CC_GEC(GVars,_shared,"n/GVars");
  auto puck = CC_GEC(Puck,_puck,"f/CPixie");
  auto bc= puck->circum();
  auto wb= cx::visBox();
  //check for goals!

  if (pv->moveTarget.y > wb.top + bc) {
    auto msg=j::json({
        {"pnum", 1},
        {"score",1}
        });
    SENDMSGEX("/game/player/earnscore",&msg);
  }
  else
  if (pv->moveTarget.y < -bc) {
    auto msg=j::json({
        {"pnum", 2},
        {"score",1}
        });
    SENDMSGEX("/game/player/earnscore",&msg);
  }

  //move pieces to next position

  CC_POS1(puck, pv->moveTarget);
  F__LOOP(it,_mallets) {
    auto &node= *it;
    auto mv=CC_GEC(f::CMove,node,"f/CMove");
    auto m=CC_GEC(Mallet,node,"f/CPixie");
    CC_POS1(m, mv->moveTarget);
  }

}


NS_END




