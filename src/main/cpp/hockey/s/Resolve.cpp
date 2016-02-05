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
NS_BEGIN(hockey)

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::preamble() {
  shared= engine->getNodeList(SharedNode().typeId());
  pucks= engine->getNodeList(PuckNode().typeId());
  mallets= engine->getNodeList(MalletNode().typeId());
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
  auto ss = CC_GNLF(GVars,shared,"slots");
  auto puck = CC_GNLF(Puck,pucks,"puck");
  auto bc= puck->circum();
    auto wb= cx::visBox();
  //check for goals!

  if (puck->nextPos.y > wb.top + bc) {
    auto msg=j::json({
        {"pnum", 1},
        {"score",1}
        });
    SENDMSGEX("/game/player/earnscore",&msg);
  }
  else
  if (puck->nextPos.y < -bc) {
    auto msg=j::json({
        {"pnum", 2},
        {"score",1}
        });
    SENDMSGEX("/game/player/earnscore",&msg);
  }

  //move pieces to next position
  for(auto node=mallets->head;node;node=node->next) {
    auto m=CC_GNF(Mallet,node,"mallet");
    m->syncPos();
  }

  puck->syncPos();
}



NS_END




