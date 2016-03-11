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
#include "core/CCSX.h"
#include "n/lib.h"
#include "Clear.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////////
//
void Clear::preamble() {
  _arena = _engine->getNodes("n/BlockGrid")[0];
}

//////////////////////////////////////////////////////////////////////////////
//
bool Clear::update(float dt) {
  auto rc=true;
  if (MGMS()->isLive()) {
    auto ps= CC_GEC(Pauser, _arena, "n/Pauser");
    // while we pause, clear the filled lines, disallow
    // other things => return false
    if (ps->pauseToClear) {
      if (cx::timerDone(ps->timer)) {
        cx::undoTimer(ps->timer);
        ps->timer=CC_NIL;
        clearFilled();
        ps->pauseToClear=false;
      }
      rc= false;
    }
  }
  return rc;
}

//////////////////////////////////////////////////////////////////////////////
//
void Clear::clearFilled() {
  auto flines = CC_GEC(FilledLines, _arena, "n/FilledLines");
  auto score= flines->lines.size();

  F__LOOP(it, flines->lines) {
    auto z = *it;
    clearOneRow( z);
    resetOneRow( z);
  }

  auto msg= j::json({
      { "score", score * 50 }
      });
  shiftDownLines();
  SENDMSGEX("/hud/score/update", &msg);
}

//////////////////////////////////////////////////////////////////////////////
//
void Clear::clearOneRow( int r) {
  auto bks= CC_GEC(BlockGrid, _arena, "n/BlockGrid");
  auto &row= bks->grid[r];
  auto last = row.size() - 1;

  // skip the 2 side walls
  for (auto c=1; c < last; ++c) {
    auto z= row.get(c);
    if (z) { z->dispose(); }
    delete z;
    row.set(c, CC_NIL);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Clear::resetOneRow( int r) {
}

//////////////////////////////////////////////////////////////////////////////
//
void Clear::shiftDownLines() {
  auto top= topLine(_arena);
  int f0,e0;
  // top down search
  f0= findFirstDirty();
  // no lines are touched
  if (f0==0){
    return;
  }
  e0= findFirstEmpty(f0);
  if (e0==0) {
    return;
  }
  auto e= findLastEmpty(e0);
  auto f = e0 + 1; // last dirty
  if (f > f0) {
    throw "error while shifting lines down";
  }
  auto r= (e0 - e) + 1;  // number of empty lines
  auto s=f;  // last dirty
  auto t=e;
  while (s <= f0) {
    copyLine(s, t);
    ++t;
    ++s;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
int Clear::findFirstDirty() {
  auto t = topLine(_arena);// - 1,

  for (auto r = t; r > 0; --r) {
    if (!isEmptyRow(r)) { return r; }
  }

  return 0;
}

//////////////////////////////////////////////////////////////////////////////
//
int Clear::findFirstEmpty( int t) {
  for (auto r=t; r > 0; --r) {
    if (isEmptyRow(r)) { return r; }
  }
  return 0;
}

//////////////////////////////////////////////////////////////////////////////
//
int Clear::findLastEmpty( int t) {
  for (auto r=t; r >= 0; --r) {
    if (!isEmptyRow(r)) { return r+1; }
  }
  //should never get here
  throw "findLastEmpty has error";
  //return 1;
}

//////////////////////////////////////////////////////////////////////////////
//
bool Clear::isEmptyRow( int r) {
  auto bs= CC_GEC(BlockGrid, _arena, "n/BlockGrid");
  auto &row= bs->grid[r];
  auto len= row.size() - 1;

  if (r==0) { return false; }

  for (auto c=1; c < len; ++c) {
    if (row.get(c) != CC_NIL) { return false; }
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Clear::copyLine( int from, int to) {
  auto bs = CC_GEC(BlockGrid, _arena, "n/BlockGrid");
  auto tile = CC_CSV(c::Float, "TILE");
  auto &g_f = bs->grid[from];
  auto &g_t = bs->grid[to];
  auto end= g_f.size() -1;
  auto dlen= tile * (from - to);

  // skip the 2 side walls
  for (auto c=1; c < end; ++c) {
    auto b= g_f.get(c);
    if (b) {
      auto pos = b->node->getPosition();
      b->node->setPosition(pos.x, pos.y - dlen);
    }
    auto old= g_t.swap(c, b);
    if (old) {
      old->dispose();
      delete old;
    }
    g_f.set(c, CC_NIL);
  }
}


NS_END


