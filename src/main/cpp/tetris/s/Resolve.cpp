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
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "n/lib.h"
#include "Resolve.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tetris)


//////////////////////////////////////////////////////////////////////////
//
void Resolve::preamble() {
  arena = engine->getNodeList(ArenaNode().typeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Resolve::update(float dt) {

  if (MGMS()->isLive()) {
    doIt();
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::doIt() {

  auto sh = CC_GNLF(ShapeShell, arena, "shell");
  auto bs= CC_GNLF(BlockGrid, arena, "blocks");
  auto mo = CC_GNLF(Gesture, arena, "motion");
  auto shape= sh->shape;
  auto &emap= bs->grid;

  if (ENP(shape)) {
    return;
  }

  if ( mo->right) {
    shiftRight( MGML(), emap, shape);
  }

  if ( mo->left) {
    shiftLeft( MGML(), emap, shape);
  }

  if ( mo->rotr) {
    rotateRight( MGML(), emap, shape);
  }

  if ( mo->rotl) {
    rotateLeft( MGML(), emap, shape);
  }

  if ( mo->down) {
    fastDrop();
  }

  mo->reset();
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::fastDrop() {
  auto dp = CC_GNLF(Dropper, arena, "dropper");
  auto cfg = MGMS()->getLCfg()->getValue();
  cx::undoTimer(dp->timer);
  // drop at fast-drop rate
  dp->dropSpeed= JS_FLOAT(cfg["speed"]);
  dp->dropRate = JS_FLOAT(cfg["drate"]);
  setDropper(MGML(), dp, dp->dropRate, dp->dropSpeed);
}


NS_END(tetris)


