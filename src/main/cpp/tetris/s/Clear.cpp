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

#include "core/CCSX.h"
#include "Clear.h"
#include "s/utils.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////////
//
Clear::Clear(not_null<EFactory*> f, not_null<c::Dictionary*> d)

  : XSystem<EFactory>(f, d) {

}

//////////////////////////////////////////////////////////////////////////////
//
void Clear::addToEngine(not_null<a::Engine*> e) {
  ArenaNode n;
  arena= e->getNodeList(n.typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Clear::update(float dt) {
  auto n= arena->head;

  if (MGMS()->isLive()) {

    auto ps= CC_GNF(Pauser, n, "pauser");
    if (ps->pauseToClear) {
      if (cx::timerDone(ps->timer)) {
        clearFilled(n);
        cx::undoTimer(ps->timer);
        ps->timer=nullptr;
        ps->pauseToClear=false;
      }
      //stop downstream processing
      return false;
    }
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Clear::clearFilled(a::Node *node) {
  auto flines = CC_GNF(FilledLines, node, "flines");
  auto score= flines->lines.size();

  F__LOOP(it, flines->lines) {
    clearOneRow(node, *it);
    resetOneRow(node, *it);
  }

  shiftDownLines(node);
  sendEx("/hud/score/update", j::json({
      { "score", score * 50 }
      }));
}

//////////////////////////////////////////////////////////////////////////////
//
void Clear::clearOneRow(a::Node *node, int r) {
  auto bks= CC_GNF(BlockGrid, node, "blocks");
  auto& row= bks->grid[r];

  for (auto c=0; c < row.size(); ++c) {
    auto z= row.get(c);
    if (NNP(z)) {
      row.set(c, nullptr);
      z->dispose();
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Clear::resetOneRow(a::Node *node, int r) {
  auto co= CC_GNF(TileGrid, node, "collision");
  auto &row= co->tiles[r];

  for (auto c=0; c < row.size(); ++c) {
    row.set(c, r==0 ? 1 : 0);
  }
  row.setFirst(1);
  row.setLast(1);
}

//////////////////////////////////////////////////////////////////////////////
//
void Clear::shiftDownLines(a::Node *node) {
  auto top= topLine(node);
  // top down search
  auto f0= findFirstDirty(node);
  // no lines are touched
  if (f0==0) { return; }
  auto e0= findFirstEmpty(node,f0);
  if (e0==0) { return; }
  auto e= findLastEmpty(node, e0);
  auto f = e0 + 1; // last dirty
  if (f > f0) {
    throw "error while shifting lines down";
  }
  auto r= (e0 - e) + 1;  // number of empty lines
  auto s=f;  // last dirty
  auto t=e;
  while (s <= f0) {
    copyLine(node, s, t);
    ++t;
    ++s;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
int Clear::findFirstDirty(a::Node *node) {
  auto t = topLine(node);// - 1,

  for (auto r = t; r > 0; --r) {
    if (!isEmptyRow(node,r)) { return r; }
  }

  return 0;
}

//////////////////////////////////////////////////////////////////////////////
//
int Clear::findFirstEmpty(a::Node *node, int t) {
  for (auto r=t; r > 0; --r) {
    if (isEmptyRow(node,r)) { return r; }
  }
  return 0;
}

//////////////////////////////////////////////////////////////////////////////
//
int Clear::findLastEmpty(a::Node *node, int t) {
  for (auto r=t; r >= 0; --r) {
    if (!isEmptyRow(node,r)) { return r+1; }
  }
  //should never get here
  throw "findLastEmpty has error";
  //return 1;
}

//////////////////////////////////////////////////////////////////////////////
//
bool Clear::isEmptyRow(a::Node *node, int r) {
  auto co= CC_GNF(TileGrid, node, "collision");
  auto &row= co->tiles[r];
  auto len= row.size() - 1;

  if (r==0) { return false; }

  for (auto c=1; c < len; ++c) {
    if (row.get(c) != 0) { return false; }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Clear::copyLine(a::Node *node, int from, int to) {
  auto co = CC_GNF(TileGrid, node, "collision");
  auto bs = CC_GNF(BlockGrid, node, "blocks");
  auto tile = CC_CSV(c::Integer, "TILE");
  auto &line_f = co->tiles[from];
  auto &line_t = co->tiles[to];
  auto dlen= tile * (from - to);
  int c;

  for (c=0; c < line_f.size(); ++c) {
    line_t.set(c, line_f.get(c));
    line_f.set(c, 0);
  }
  line_f.setFirst(1);
  line_f.setLast(1);

  // deal with actual shape
  auto &g_f = bs->grid[from];
  auto &g_t = bs->grid[to];

  for (c=0; c < g_f.size(); ++c) {
    if (g_f.get(c)) {
      auto pos = g_f.get(c)->getPosition();
      g_f.get(c)->setPosition(pos.x, pos.y - dlen);
    }
    g_t.set(c, g_f.get(c));
    g_f.set(c, nullptr);
  }
}


NS_END(tetris)


