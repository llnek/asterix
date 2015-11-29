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

#include "EFactory.h"
#include "Stager.h"
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
Stager::Stager(not_null<EFactory*> f,
    not_null<c::Dictionary*> d)

  : f::BaseSystem<EFactory>(f, d) {

  this->inited=false;
  SNPTR(ships)
}

//////////////////////////////////////////////////////////////////////////////
//
void Stager::removeFromEngine(not_null<a::Engine*> e) {
  SNPTR(ships)
}

//////////////////////////////////////////////////////////////////////////////
//
void Stager::addToEngine(not_null<a::Engine*> e) {
  ShipMotionNode n;
  ships = e->getNodeList(n.typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
bool Stager::onUpdate(float dt) {
  if (cx::isTransitioning()) { return false; }
  if (NNP(ships->head)) {
    if (! inited) {
      onceOnly();
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Stager::onceOnly() {

  MGMS()->reifyPool("BackTiles");
  MGMS()->reifyPool("BackSkies");

  MGMS()->reifyPool("Missiles");
  MGMS()->reifyPool("Baddies");
  MGMS()->reifyPool("Bombs");

  MGMS()->reifyPool("Explosions");
  MGMS()->reifyPool("Sparks");
  MGMS()->reifyPool("HitEffects");

  factory->createBackSkies();

  sharedExplosion();
  initBackSkies();

  factory->createBackTiles();
  MGMS()->sendMsg("/game/backtiles");

  factory->createMissiles();
  factory->createBombs();
  factory->createEnemies();

  factory->createExplosions();
  factory->createSparks();
  factory->createHitEffects();

  bornShip(ships->head->ship);
}


  /**
   * @method fire
   * @private
   */
  fire(t, evt) {
    if ('/touch/one/move' === t ||
        '/mouse/move' === t) {} else {
      return;
    }
    if (this.state.running &&
        !!this.ships.head) {
      let ship = this.ships.head.ship,
      pos = ship.pos(),
      wz= ccsx.vrect(),
      cur= cc.pAdd(pos, evt.delta);
      cur= cc.pClamp(cur, cc.p(0, 0),
                     cc.p(wz.width, wz.height));
      ship.setPos(cur.x, cur.y);
    }
  }

}, {

/**
 * @memberof module:s/stager~Stager
 * @property {Number} Priority
 */
Priority : xcfg.ftypes.PreUpdate
};


  /**
   * @method initBackSkies
   * @private
   */
  initBackSkies() {
    const bs = sh.pools.BackSkies.get();
    bs.inflate({x: 0, y: 0});
    this.state.backSkyRe = null;
    this.state.backSky = bs;
    this.state.backSkyDim = cc.size(bs.size());
  },
  /**
   * @method sharedExplosion
   * @private
   */
  sharedExplosion() {
    let animFrames = [],
    animation,
    frame;

    for (let n = 1; n < 35; ++n) {
      let str = "explosion_" + (n < 10 ? ("0" + n) : n) + ".png";
      frame = ccsx.getSprite(str);
      animFrames.push(frame);
    }
    animation = new cc.Animation(animFrames, 0.04);
    cc.animationCache.addAnimation(animation, "Explosion");
  },

NS_END(terra)

