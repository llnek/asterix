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
  mallets= engine->getNodeList(MalletNode().typeId());
  pucks= engine->getNodeList(PuckNode().typeId());
  shared= engine->getNodeList(SharedNode().typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::processMallets() {
  auto ss = CC_GNLF(GVars,shared,"slots");
  auto puck = CC_GNLF(Puck,pucks,"puck");
  auto gw2= ss->goalWidth * 0.5f;
  auto bnps = puck->nextPos;
  auto br= puck->radius();
  auto bps= puck->pos();
  auto bvec = puck->vec * 0.98f;

  bnps.x += bvec.x;
  bnps.y += bvec.y;

  for(auto node=mallets->head;node;node=node->next) {
    auto m= CC_GNF(Mallet,node,"mallet");
    auto mr=m->radius();
    auto mnps = m->nextPos;
    auto mvec = m->vec;
    auto cp= m->pos();
    auto dx = bnps.x - cp.x;
    auto dy = bnps.y - cp.y;
    auto dist1 = dx*dx + dy*dy;
    auto dist2 = pow(bps.x - mnps.x, 2) + pow(bps.y - mnps.y, 2);

    if (dist1 <= ss->sq_radii || dist2 <= ss->sq_radii) {
      auto mag_player = mvec.x*mvec.x + mvec.y*mvec.y;
      auto mag_ball = bvec.x*bvec.x + bvec.y*bvec.y;
      auto force = sqrt(mag_ball + mag_player);
      auto angle = atan2(dy, dx);

      bvec.x = force* cos(angle);
      bvec.y = force* sin(angle);

      bnps.x = mnps.x +
        (mr + br + force) * cos(angle);
      bnps.y = mnps.y +
        (mr + br + force) * sin(angle);

      cx::sfxPlay("hit");
    }
  }

  //check collision of ball and sides
  if (bnps.x > wb.right - br) {
    bnps.x = wb.right - br;
    bvec.x *= -0.8f;
    cx::sfxPlay("hit");
  }
  if (bnps.x < br) {
    bnps.x = br;
    bvec.x *= -0.8f;
    cx::sfxPlay("hit");
  }

  //ball and top of the court
  if (bnps.y > wb.top - br) {
    if (bps.x < wb.cx - gw2 ||
      bps.x > wb.cx + gw2) {
      bnps.y = wb.top - br;
      bvec.y *= -0.8f;
      cx::sfxPlay("hit");
    }
  }
  //ball and bottom of the court
  if (bnps.y < br) {
    if (bps.x < wb.cx - gw2 || bps.x > wb.cx + gw2) {
      bnps.y = br;
      bvec.y *= -0.8f;
      cx::sfxPlay("hit");
    }
  }

  //finally, update ball's vector and next position
  puck->nextPos = bnps;
  puck->vec = bvec;

}


NS_END




