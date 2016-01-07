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
#include "Game.h"
#include "Stage.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
//
void Stage::preamble() {
  ShipNode n;
  shipNode = engine->getNodeList(n.typeId());
  assert(shipNode->head != nullptr);
  onceOnly();
}

//////////////////////////////////////////////////////////////////////////////
//
bool Stage::update(float dt) {
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Stage::onceOnly() {
  bornShip(CC_GNLF(Ship,shipNode,"ship"));
  sharedExplosion();
  initBackSkies();
  SENDMSG("/game/backtiles");
}

//////////////////////////////////////////////////////////////////////////
//
void Stage::initBackSkies() {
  auto p = MGMS()->getPool("BackSkies");
  auto g = (Game*) MGMS();
  auto bs = p->get();

  assert(bs != nullptr);
  bs->inflate(0, 0);

  g->backSkyDim = bs->csize();
  g->backSkyRe = nullptr;
  g->backSky = bs;
}

//////////////////////////////////////////////////////////////////////////
//
void Stage::sharedExplosion() {
  c::Vector<c::SpriteFrame*> fs;
  sstr str;
  for (auto n = 1; n < 35; ++n) {
    auto ns= s::to_string(n);
    if (n < 10 ) { str= "0" + ns; } else { str= ns; }
    fs.pushBack( cx::getSpriteFrame( "explosion_" + str + ".png"));
  }
  auto a = c::Animation::createWithSpriteFrames(fs, 0.04f);
  c::AnimationCache::getInstance()->addAnimation(a, "Explosion");
}


NS_END(terra)

