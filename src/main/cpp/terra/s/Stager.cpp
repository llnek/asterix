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

#include "Stager.h"
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
//
Stager::Stager(not_null<EFactory*> f,
    not_null<c::Dictionary*> d)
  :
  BaseSystem<EFactory>(f, d) {

  this->inited=false;
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
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Stager::onceOnly() {

  factory->createBackSkies();
  factory->createBackTiles();

  sharedExplosion();
  initBackSkies();

  bornShip(ships->head->ship);
  MGMS()->sendMsg("/game/backtiles");
}

/*
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
*/

//////////////////////////////////////////////////////////////////////////
//
void Stager::initBackSkies() {
  auto p = MGMS()->getPool("BackSkies");
  auto g = (Game*) MGMS();
  auto bs = p->get();

  assert(bs != nullptr);
  bs->inflate(0, 0);

  g->backSkyDim = bs->size();
  g->backSkyRe = nullptr;
  g->backSky = bs;
}

//////////////////////////////////////////////////////////////////////////
//
void Stager::sharedExplosion() {
  c::Vector<AnimationFrame*> fs;

  for (auto n = 1; n < 35; ++n) {
    auto str = "explosion_" + (n < 10 ? ("0" + n) : n) + ".png";
    fs.pushBack( cx::getSpriteFrame(str));
  }

  auto a = c::Animation::create();
  a->setFrames(fs);
  a->setDelayPerUnit(0.04f);

  c::AnimationCache::getInstance()->addAnimation(a, "Explosion");
}


NS_END(terra)

