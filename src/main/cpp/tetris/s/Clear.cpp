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

#include "Clear.h"

NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////////
//
Clear::Clear(not_null<EFactory*> f, not_null<c::Dictionary*> d)

  : BaseSystem<EFactory>(f, d) {

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
  auto node = arena->head;

  if (MGMS()->isLive() &&
      NNP(node)) {
    auto ps= CC_GNF(Pauser, node, "pauser");
    if (ps->pauseToClear) {
      if (cx::timerDone(ps->timer)) {
        clearFilled(node);
        ps->timer=cx::undoTimer(ps->timer);
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
  j::json msg({
      { "score", score * 50 }
      });
  MGMS()->sendMsgEx("/hud/score/update", &msg);
}

//////////////////////////////////////////////////////////////////////////////
//
void Clear::clearOneRow(a::Node *node, int r) {
  auto bks= CC_GNF(BlockGrid, node, "blocks");
  auto& row= bks->grid[r];

  for (auto c=0; c < row.size(); ++c) {
    auto z= row->get(c);
    if (NNP(z)) {
      row->set(c, nullptr);
      z->dispose();
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Clear::resetOneRow(a::Node *node, int r) {
  auto co= CC_GNF(TileGrid, node, "collision");
  auto row= co->tiles[r];

  for (auto c=0; c < row.size(); ++c) {
    row->set(c, r==0 ? 1 : 0);
  }
  row->setFirst(1);
  row->setLast(1);
}


  /**
   * @method shiftDownLines
   * @private
   */
  shiftDownLines(node) {
    let top= utils.topLine(node),
    f0, f, e0, e, d,
    s, t, r;

    // top down search

    f0= this.findFirstDirty(node);
    // no lines are touched
    if (f0===0) { return; }
    e0= this.findFirstEmpty(node,f0);
    if (e0===0) { return; }
    e= this.findLastEmpty(node, e0);
    f = e0 + 1; // last dirty
    if (f > f0) {
      sjs.tne("error while shifting lines down");
      return;
    } // error!
    r= (e0 - e) + 1;  // number of empty lines

    s=f;  // last dirty
    t=e;
    while (s <= f0) {
      this.copyLine(node, s, t);
      ++t;
      ++s;
    }
  },
  /**
   * @method findFirstDirty
   * @private
   */
  findFirstDirty(node) {
    const t = utils.topLine(node);// - 1,

    for (let r = t; r > 0; --r) {
      if (!this.isEmptyRow(node,r)) { return r; }
    }

    return 0;
  },
  /**
   * @method findFirstEmpty
   * @private
   */
  findFirstEmpty(node, t) {
    for (let r=t; r > 0; --r) {
      if (this.isEmptyRow(node,r)) { return r; }
    }
    return 0;
  },
  /**
   * @method findLastEmpty
   * @private
   */
  findLastEmpty(node,t) {
    for (let r=t; r >= 0; --r) {
      if (!this.isEmptyRow(node,r)) { return r+1; }
    }
    //should never get here
    sjs.tne("findLastEmpty has error.");
    return 1;
  },
  /**
   * @method isEmptyRow
   * @private
   */
  isEmptyRow(node, r) {
    const row= node.collision.tiles[r],
    len= row.length-1;

    if (r===0) { return false; }

    for (let c=1; c < len; ++c) {
      if (row[c] !== 0) { return false; }
    }
    return true;
  },
  /**
   * @method copyLine
   * @private
   */
  copyLine(node, from, to) {
    let line_f = node.collision.tiles[from],
    line_t = node.collision.tiles[to],
    dlen= csts.TILE * (from - to),
    c, pos;

    for (c=0; c < line_f.length; ++c) {
      line_t[c] = line_f[c];
      line_f[c]= 0;
    }
    line_f[0]=1;
    line_f[line_f.length-1]=1;

    // deal with actual shape
    line_f = node.blocks.grid[from];
    line_t = node.blocks.grid[to];

    for (c=0; c < line_f.length; ++c) {
      if (line_f[c]) {
        pos = line_f[c].sprite.getPosition();
        line_f[c].sprite.setPosition(pos.x, pos.y - dlen);
      }
      line_t[c] = line_f[c];
      line_f[c] = undef;
    }
  }



NS_END(tetris)


