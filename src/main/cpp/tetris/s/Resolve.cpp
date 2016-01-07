// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2015, Ken Leung. All rights reserved.

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "s/utils.h"
#include "Resolve.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tetris)


//////////////////////////////////////////////////////////////////////////
//
void Resolve::preamble() {
  arenaNode = engine->getNodeList(ArenaNode().typeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Resolve::update(float dt) {

  if (MGMS()->isLive()) {
    doIt();
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::doIt() {

  auto co= CC_GNLF(TileGrid, arenaNode, "collision");
  auto sh = CC_GNLF(ShapeShell, arenaNode, "shell");
  auto mo = CC_GNLF(Motion, arenaNode, "motion");
  auto &cmap= co->tiles;
  auto shape= sh->shape;

  if (ENP(shape)) {
    return;
  }

  if (MGML()->keyPoll(KEYCODE::KEY_RIGHT_ARROW) ||
      mo->right) {
    shiftRight( MGML(), cmap, shape);
  }

  if (MGML()->keyPoll(KEYCODE::KEY_LEFT_ARROW) ||
      mo->left) {
    shiftLeft( MGML(), cmap, shape);
  }

  if (MGML()->keyPoll(KEYCODE::KEY_DOWN_ARROW) ||
      mo->rotr) {
    rotateRight( MGML(), cmap, shape);
  }

  if (MGML()->keyPoll(KEYCODE::KEY_UP_ARROW) ||
      mo->rotl) {
    rotateLeft( MGML(), cmap, shape);
  }

  if (MGML()->keyPoll(KEYCODE::KEY_SPACE) ||
      mo->down) {
    fastDrop();
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::fastDrop() {
  auto dp = CC_GNLF(Dropper, arenaNode, "dropper");
  auto cfg = MGMS()->getLCfg()->getValue();
  cx::undoTimer(dp->timer);
  // drop at fast-drop rate
  dp->dropSpeed= JS_FLOAT(cfg["speed"]);
  dp->dropRate = JS_FLOAT(cfg["drate"]);
  setDropper(MGML(), dp, dp->dropRate, dp->dropSpeed);
}


NS_END(tetris)


