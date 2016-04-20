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
#include "C.h"

NS_BEGIN(cuteness)

//////////////////////////////////////////////////////////////////////////////
//
void scanAndSmash(EnemySquad *sq, const CCT_PT &tap) {
  auto &ps= sq->getPools();
  F__LOOP(it, ps) {
    auto &p= *it;
    auto &objs= p->ls();
    F__LOOP(it2, objs) {
      auto &e = *it2;
      if (!e->status()) { continue; }
      auto h=CC_GEC(f::CHealth,e,"f/CHealth");
      auto sp=CC_GEC(Enemy,e,"f/CPixie");
      if (sp->boundingBox().containsPoint(tap)) {
        h->hurt();
      }
      if (!h->alive()) {
        auto msg= j::json({
            {"score", 10}
            });
        cx::hibernate((ecs::Node*)e);
        cx::sfxPlay("sndKill");
        SENDMSGEX("/game/player/earnscore", &msg);
      }
    }
  }
}


NS_END




