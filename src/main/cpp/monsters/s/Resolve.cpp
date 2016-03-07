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
NS_BEGIN(monsters)

//////////////////////////////////////////////////////////////////////////////
//
void Resolve::preamble() {
  auto a1= engine->getEntities("n/Automa");
  auto a2= engine->getEntities("f/CHuman");
  assert(a1.size()==1);
  assert(a2.size()==1);
  _human=a2[0];
  _enemy=a1[0];
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
  auto h= CC_GEC(f::CHealth,_human,"f/CHealth");

  if (h->curHp <= 0) {
    // gameover
    SENDMSG("/game/player/lose");
    return;
  }

  auto he= CC_GEC(f::CHealth,_enemy,"f/CHealth");
  if (he->curHp <= 0) {
    SENDMSG("/game/player/lose");
    return;
  }

  // Display coins
  auto humanPlayer = CC_GEC(Stash,_human,"n/Stash");
  auto msg
  getHUD()->updateCoins(1, humanPlayer->coins);
  auto aiPlayer = CC_GEC(Stash,_enemy,"n/Stash");
  getHUD()->updateCoins(2, aiPlayer->coins);

  // Display AI state
  auto ai = CC_GEC(Automa,pe,"n/Automa");
  getHUD()->updateAIState(ai->state->name());
}


NS_END




