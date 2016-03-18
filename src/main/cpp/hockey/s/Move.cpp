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
NS_BEGIN(hockey)

//////////////////////////////////////////////////////////////////////////////
//
void Move::preamble() {
  _shared = _engine->getNodes("n/GVars")[0];
  _puck = _engine->getNodes("f/CAutoma")[0];
  _engine->getNodes("f/CGesture", _mallets);
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
  auto pv= CC_GEC(f::CMove,_puck,"f/CMove");
  auto ss= CC_GEC(GVars,_shared,"n/GVars");
  auto puck= CC_GEC(Puck,_puck,"f/CPixie");
  auto bnpos = pv->moveTarget;
  auto br= puck->radius();
  auto bv = pv->vel;
  bv *=  0.98;

  bnpos.x += bv.x;
  bnpos.y += bv.y;

  F__LOOP(it,_mallets) {
    auto node= *it;
    auto mv=CC_GEC(f::CMove,node,"f/CMove");
    auto m=CC_GEC(Mallet,node,"f/CPixie");
    auto pnpos = mv->moveTarget;
    auto bpos= puck->pos();
    auto mpos= m->pos();
    auto mr= m->radius();
    auto pv = mv->vel;
    auto dx = bnpos.x - mpos.x;
    auto dy = bnpos.y - mpos.y;
    auto dist1 = pow(dx, 2) + pow(dy, 2);
    auto dist2 = pow(bpos.x - pnpos.x, 2) + pow(bpos.y - pnpos.y, 2);

    if (dist1 <= ss->sq_radii || dist2 <= ss->sq_radii) {

      auto mag_player = pow(pv.x, 2) + pow(pv.y, 2);
      auto mag_ball = pow(bv.x, 2) + pow(bv.y, 2);
      auto force = sqrt(mag_ball + mag_player);
      auto angle = atan2(dy, dx);

      bv.x = force* cos(angle);
      bv.y = force* sin(angle);

      bnpos.x = pnpos.x + (mr + br + force) * cos(angle);
      bnpos.y = pnpos.y + (mr + br + force) * sin(angle);

      cx::sfxPlay("hit");
    }
  }

  ss->ballNextPos= bnpos;
  ss->ballVec= bv;

}









NS_END




