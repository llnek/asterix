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

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "s/utils.h"
#include "Resolve.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////
//
Resolve::Resolve(not_null<EFactory*> f, not_null<c::Dictionary*> d)

: XSystem<EFactory>(f, d) {

}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::addToEngine(not_null<a::Engine*> e) {
  ArenaNode n;
  arena = e->getNodeList(n.typeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Resolve::update(float dt) {
  auto n= arena->head;

  if (MGMS()->isLive()) {
    doIt(n);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::doIt(a::Node *node) {

  auto motion = CC_GNF(Motion, node, "motion");
  auto sh = CC_GNF(ShapeShell, node, "shell");
  auto co= CC_GNF(TileGrid, node, "tiles");
  auto &cmap= co->tiles;
  auto shape= sh->shape;

  if (NNP(shape)) {
    if (motion->right) {
      shiftRight( MGML(), cmap, shape);
    }
    if (motion->left) {
      shiftLeft( MGML(), cmap, shape);
    }
    if (motion->rotr) {
      rotateRight( MGML(), cmap, shape);
    }
    if (motion->rotl) {
      rotateLeft( MGML(), cmap, shape);
    }
    if (motion->down) {
      fastDrop( node);
    }
  }
  motion->right = false;
  motion->left = false;
  motion->rotr = false;
  motion->rotl = false;
  motion->down = false;
}

//////////////////////////////////////////////////////////////////////////
//
void Resolve::fastDrop(a::Node *node) {
  auto dp = CC_GNF(Dropper, node, "dropper");
  dp->timer=nullptr;
  setDropper(MGML(), dp, dp->dropRate, 9000.0f);
}


NS_END(tetris)


