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
#include "n/lib.h"
#include "Move.h"

NS_BEGIN(tetris)


//////////////////////////////////////////////////////////////////////////
//
void Move::preamble() {
  arena = engine->getNodeList(ArenaNode().typeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Move::update(float dt) {

  if (MGMS()->isLive()) {
    auto sh= CC_GNLF(ShapeShell, arena, "shell");
    auto dp= CC_GNLF(Dropper, arena, "dropper");
    if (cx::timerDone(dp->timer) &&
        NNP(sh->shape)) {
      cx::undoTimer(dp->timer);
      dp->timer=nullptr;
      doFall();
    }
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Move::doFall() {
  auto sh= CC_GNLF(ShapeShell, arena, "shell");
  auto bs= CC_GNLF(BlockGrid, arena, "blocks");
  auto dp= CC_GNLF(Dropper, arena, "dropper");
  auto pu= CC_GNLF(Pauser, arena, "pauser");
  auto shape= sh->shape;
  auto &emap= bs->grid;

  if (NNP(shape)) {
    if (! moveDown(MGML(), emap, shape)) {

      // lock shape in place
      lock(arena->head,shape);

      /*
      //TODO: what is this???
      if (ENP(pu->timer)) {
        sh->shape=nullptr;
        shape->bricks.clear();
        delete shape;
      }
      */

      shape->bricks.clear();
      sh->shape=nullptr;
      mc_del_ptr(shape);

    } else {
      // drop at fast-drop rate
      setDropper(MGML(), dp, dp->dropRate, dp->dropSpeed);
    }
  }
}


NS_END(tetris)


