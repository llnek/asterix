// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Collide.h"
#include "Move.h"
#include "AI.h"
#include "GEngine.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(skyfox)

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createMeteors(int cnt) {
  auto po= MGMS()->getPool("Meteors");
  po->preset([=]() -> f::Poolable* {
    auto m= Meteor::create();
    auto e= this->reifyNode("Meteor");
    MGML()->addAtlasItem(
        "game-pics",
        m, kMiddleground, kSpriteMeteor);
    CC_HIDE(m);
    e->checkin(m);
    return e;
  }, cnt);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createHealths(int cnt) {
  auto po= MGMS()->getPool("Healths");
  po->preset([=]() -> f::Poolable* {
    auto h= Health::create();
    auto e= this->reifyNode("Health");
    h->setAnchorPoint(CCT_PT(0.5, 0.8));
    MGML()->addAtlasItem(
        "game-pics",
        h, kMiddleground, kSpriteHealth);
    CC_HIDE(h);
    e->checkin(h);
    return e;
  }, cnt);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {

  MGMS()->reifyPools(s_vec<sstr> {"Meteors", "Healths", "Clouds" });

  auto ent= this->reifyNode("Arena",true);
  auto ss= mc_new(GVars);
  auto wb= cx::visBox();
  ent->checkin(ss);

  auto bomb = Bomb::create();
  MGML()->addAtlasItem("game-pics", bomb);
  ent= this->reifyNode("Bomb",true);
  ent->checkin(bomb);
  ent->checkin(mc_new(f::CGesture));

  //add shockwave
  ss->shockWave = cx::reifySprite("shockwave.png");
  ss->shockWave->getTexture()->generateMipmap();
  CC_HIDE(ss->shockWave);
  MGML()->addAtlasItem("game-pics", ss->shockWave);

  //blink ray
  ss->blinkRay = c::RepeatForever::create(
      c::Blink::create(4, 6));
  CC_KEEP(ss->blinkRay)

  auto anim= cx::createAnimation(1.0/4.0);
  for (auto i = 1; i <= 4; ++i) {
    anim->addSpriteFrame(
        cx::getSpriteFrame(
          "ufo_" + FTOS(i) +".png"));
  }
  ss->ufoAnimation = c::Animate::create(anim);
  CC_KEEP(ss->ufoAnimation)

  auto ufo= Ufo::create();
  MGML()->addAtlasItem("game-pics", ufo, 0, kSpriteUfo);
  ent=this->reifyNode("Ufo", true);
  ent->checkin(ufo);
  ent->checkin(mc_new(f::CAutoma));

  createMeteors();
  createHealths();
  createClouds();
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createClouds(int cnt) {
  auto po=MGMS()->reifyPool("Clouds");
  auto wb=cx::visBox();
  for (auto i = 0; i < 4; ++i) {
    auto y = wb.top * (i % 2 == 0 ? 0.4 : 0.5);
    auto c = Cloud::create();
    auto e = this->reifyNode("Cloud",true);
    CC_POS2(c, wb.right * 0.1 + i * wb.right * 0.3, y);
    MGML()->addAtlasItem("game-pics", c, kBackground);
    e->checkin(c);
    po->checkin(e);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initSystems() {
  regoSystem(mc_new1(Collide,this));
  regoSystem(mc_new1(AI,this));
  regoSystem(mc_new1(Move,this));
}


NS_END

