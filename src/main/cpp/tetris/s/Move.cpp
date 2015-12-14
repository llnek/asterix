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

#include "s/utils.h"
#include "Move.h"

NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////
//
Move::Move(not_null<EFactory*> f, not_null<c::Dictionary*> d)

  : BaseSystem<EFactory>(f, d) {

}

//////////////////////////////////////////////////////////////////////////
//
void Move::addToEngine(not_null<a::Engine*> e) {
  ArenaNode n;
  arena = e->getNodeList(n.typeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Move::update(float dt) {
  auto node= arena->head;
  if (MGMS()->isLive() &&
      NNP(node)) {
    auto sh= CC_GNF(ShapeShell,node, "shell");
    auto dp= CC_GNF(Dropper,node, "dropper");
    if (cx::timerDone(dp->timer) &&
        NNP(sh->shape)) {
      dp->timer= cx::undoTimer(dp->timer);
      doFall(node);
    }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Move::doFall(a::Node *node) {
  auto co= CC_GNF(TileGrid, node, "collision");
  auto sh= CC_GNF(ShellShape, node, "shell");
  auto bs= CC_GNF(BlockGrid, node, "blocks");
  auto shape= sh->shape;
  auto& cmap= co->tiles;
  auto& emap= bs->grid;
  auto dp= CC_GNF(Dropper, node, "dropper");
  auto pu= CC_GNF(Pauser, node, "pauser");

  if (NNP(shape)) {
    if (! moveDown(MGML(), cmap, shape)) {

      // lock shape in place
      lock(node, shape);

      //TODO: what is this???
      if (ENP(pu->timer)) {
        sh->shape=nullptr;
        shape->bricks.clear();
      }

      sh->shape=nullptr;
      shape->bricks.clear();

    } else {
      initDropper(dp);
    }
  }
}


NS_END(tetris)


