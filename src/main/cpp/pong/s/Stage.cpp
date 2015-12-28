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

#include "Stage.h"

NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
Stage::Stage(not_null<EFactory*> f, not_null<c::Dictionary*> o)
  : XSystem<EFactory>(f,o) {
}

//////////////////////////////////////////////////////////////////////////////
//
void Stage::addToEngine(not_null<a::Engine*> e) {
}

//////////////////////////////////////////////////////////////////////////////
//
bool Stage::update(float dt) {
  if (MGMS()->isLive()) {
    if (! inited) {
      onceOnly();
    }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Stage::onceOnly() {
  auto slots= CC_GNF(Slots, arenaNode->head, "slots");
  auto world = MGMS()->getEnclosureBox();
  //auto fps = CC_CSV(c::Integer,"FPS");
  auto ps = initPaddleSize();
  auto bs = initBallSize();
  auto cw= cx::center();

  // position of paddles
  // portrait
  auto p1y = floor(world.top * 0.1f + bs.height + HHZ(ps));
  auto p2y = floor(world.top * 0.9f - bs.height - HHZ(ps));
  // landscape
  auto p2x = floor(world.right - bs.width - HWZ(ps));
  auto p1x = floor(world.left + bs.width + HWZ(ps));

  slots->pz= c::Size( Math.floor(ps.width), Math.floor(ps.height));
  slots->bz= c::Size( Math.floor(bs.width), Math.floor(bs.height));
  slots->bp= c::Vec2( Math.floor(cw.x), Math.floor(cw.y));

  if (ccsx.isPortrait()) {
    slots->p1p= c::Vec2(Math.floor(cw.x), p1y);
    slots->p2p= c::Vec2(Math.floor(cw.x), p2y);
  } else {
    slots->p1p= c::Vec2(p1x, Math.floor(cw.y));
    slots->p2p= c::Vec2(p2x, Math.floor(cw.y));
  }

  inited=true;
}

/**
 * @method fire
 * @private
 */
fire(t, evt) {
  if (('/touch/one/move' === t ||
       '/mouse/move' === t) &&
      this.state.running) {}
  else
  { return; }

  for (let node= this.nodeList.head; node; node=node.next) {
    if (ccsx.isPortrait()) {
      this.processP(node,evt);
    } else {
      this.processL(node,evt);
    }
  }
},
/**
 * @method processP
 * @private
 */
processP(node, evt) {
  let pnum= node.player.pnum,
  p = node.paddle,
  pos = p.pos(),
  loc= evt.loc,
  cur,
  x=pos.x,
  y=pos.y,
  wz= ccsx.vrect();

  if (pnum === 2 && loc.y > pos.y ||
      pnum === 1 && loc.y < pos.y) {
    x = pos.x + evt.delta.x;
    cur= cc.pClamp(cc.p(x,y), cc.p(0, 0),
                   cc.p(wz.width, wz.height));
    p.setPos(cur.x, cur.y);
  }
},
/**
 * @method processL
 * @private
 */
processL(node, evt) {
  let pnum= node.player.pnum,
  p = node.paddle,
  pos = p.pos(),
  loc= evt.loc,
  cur,
  x=pos.x,
  y=pos.y,
  wz= ccsx.vrect();

  if (pnum === 2 && loc.x > pos.x ||
      pnum === 1 && loc.x < pos.x) {
      y = pos.y + evt.delta.y;
      cur= cc.pClamp(cc.p(x,y), cc.p(0, 0),
                     cc.p(wz.width, wz.height));
      p.setPos(cur.x, cur.y);
    }
  }

//////////////////////////////////////////////////////////////////////////////
//
const c::Size Stage::initPaddleSize() {
  return cx::calcSize("red_paddle.png");
}

//////////////////////////////////////////////////////////////////////////////
//
const c::Size Stage::initBallSize() {
  return cx::calcSize("pongball.png");
}


NS_END(pong)


