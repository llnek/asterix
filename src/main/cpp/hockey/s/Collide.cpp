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
#include "Collide.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(hockey)

//////////////////////////////////////////////////////////////////////////////
//
void Collide::preamble() {
  _shared= _engine->getNodes("n/GVars")[0];
  _puck= _engine->getNodes("f/CAutoma")[0];
  _engine->getNodes("f/CGesture",_mallets);
}

//////////////////////////////////////////////////////////////////////////////
//
bool Collide::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::process(float dt) {
  auto mv = CC_GEC(f::CMove,_puck,"f/CMove");
  auto ss = CC_GEC(GVars,_shared,"n/GVars");
  auto puck = CC_GEC(Puck,_puck,"f/CPixie");
    auto gw2= HTV(ss->goalWidth);
  auto br= puck->radius();
  auto bps= puck->pos();
  auto wb= cx::visBox();
  auto bnps = ss->ballNextPos;
  auto bvec = ss->ballVec;

  //check collision of ball and sides
  if (bnps.x > wb.right - br) {
    bnps.x = wb.right - br;
    bvec.x *= -0.8;
    cx::sfxPlay("hit");
  }
  if (bnps.x < br) {
    bnps.x = br;
    bvec.x *= -0.8;
    cx::sfxPlay("hit");
  }

  //ball and top of the court
  if (bnps.y > wb.top - br) {
    if (bps.x < wb.cx - gw2 ||
      bps.x > wb.cx + gw2) {
      bnps.y = wb.top - br;
      bvec.y *= -0.8;
      cx::sfxPlay("hit");
    }
  }
  //ball and bottom of the court
  if (bnps.y < br) {
    if (bps.x < wb.cx - gw2 ||
        bps.x > wb.cx + gw2) {
      bnps.y = br;
      bvec.y *= -0.8;
      cx::sfxPlay("hit");
    }
  }

  //finally, update ball's vector and next position
  mv->moveTarget = bnps;
  mv->vel = bvec;

}


NS_END




