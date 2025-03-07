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
#include "AI.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(guesses)

//////////////////////////////////////////////////////////////////////////////
//
void AI::preamble() {
  _shared= _engine->getNodes("n/GVars")[0];
}

//////////////////////////////////////////////////////////////////////////////
//
bool AI::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}


//////////////////////////////////////////////////////////////////////////////
//
void AI::process(float dt) {
  auto ss=CC_GEC(GVars,_shared,"n/GVars");

  if (!cx::timerDone(ss->checkTimer)) {
  return; }

  cx::undoTimer(ss->checkTimer);
  ss->checkTimer=CC_NIL;

  if (ss->pickedTiles[0]->pvalue !=
      ss->pickedTiles[1]->pvalue) {
    ss->pickedTiles[0]->setSpriteFrame("cover.png");
    ss->pickedTiles[1]->setSpriteFrame("cover.png");
  } else{
    ss->pickedTiles[0]->inflated=true;
    ss->pickedTiles[1]->inflated=true;
  }

  ss->pickedTiles.clear();

}



NS_END



