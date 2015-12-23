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
  PaddleNode n;
  paddle = e->getNodeList(n.typeId());
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
  auto world = MGMS()->getEnclosureBox();
  auto cw= cx::center();
  auto ps = initPaddleSize();
  auto bs = initBallSize();
  // position of paddles
  // portrait
  auto p1y = floor(world.top * 0.1f + bs.height + HHZ(ps));
  auto p2y = floor(world.top * 0.9f - bs.height - HHZ(ps));
  // landscape
  auto p2x = floor(world.right - bs.width - HWZ(ps));
  auto p1x = floor(world.left + bs.width + HWZ(ps));

  // game defaults for entities and timers.
  this->state.framespersec= cc.game.config[cc.game.CONFIG_KEY.frameRate];
  this.state.syncMillis= 3000;
  this.state.paddle= {height: Math.floor(ps.height),
           width: Math.floor(ps.width),
           speed: Math.floor(csts.PADDLE_SPEED)};
  this.state.ball= {height: Math.floor(bs.height),
         width: Math.floor(bs.width),
         x: Math.floor(cw.x),
         y: Math.floor(cw.y),
         speed: Math.floor(csts.BALL_SPEED) };
  if (ccsx.isPortrait()) {
    this.state.p1= {y: p1y, x: Math.floor(cw.x) };
    this.state.p2= {y: p2y, x: Math.floor(cw.x) };
  } else {
    this.state.p1= {x: p1x, y: Math.floor(cw.y) };
    this.state.p2= {x: p2x, y: Math.floor(cw.y) };
  }
  this.state.numpts= csts.NUM_POINTS;

  sh.factory.createPaddles(sh.main, this.state);
  sh.factory.createBall(sh.main, this.state);

  // mouse only for 1p or netplay
  if (xcfg.csts.GAME_MODE !== sh.gtypes.P2_GAME) {
    ccsx.onMouse(this);
  }
  ccsx.onTouchOne(this);
  sh.main.pkInput();
},
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
  },
  /**
   * @method initPaddleSize
   * @private
   */
  initPaddleSize() {
    return ccsx.csize('red_paddle.png');
  },
  /**
   * @method initBallSize
   * @private
   */
  initBallSize() {
    return ccsx.csize('pongball.png');
  }

}, {
/**
 * @memberof module:s/stager~Stager
 * @property {Number} Priority
 * @static
 */
Priority : xcfg.ftypes.PreUpdate
};




NS_END(pong)


