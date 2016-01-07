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
#include "s/utils.h"
#include "Move.h"

NS_BEGIN(tetris)


//////////////////////////////////////////////////////////////////////////
//
void Move::preamble() {
  arenaNode = engine->getNodeList(ArenaNode().typeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Move::update(float dt) {

  if (MGMS()->isLive()) {
    auto sh= CC_GNLF(ShapeShell, arenaNode, "shell");
    auto dp= CC_GNLF(Dropper, arenaNode, "dropper");
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
  auto co= CC_GNLF(TileGrid, arenaNode, "collision");
  auto sh= CC_GNLF(ShapeShell, arenaNode, "shell");
  auto bs= CC_GNLF(BlockGrid, arenaNode, "blocks");
  auto dp= CC_GNLF(Dropper, arenaNode, "dropper");
  auto pu= CC_GNLF(Pauser, arenaNode, "pauser");
  auto shape= sh->shape;
  auto &cmap= co->tiles;
  auto &emap= bs->grid;

  if (NNP(shape)) {
    if (! moveDown(MGML(), cmap, shape)) {

      // lock shape in place
      lock(arenaNode->head,shape);

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


