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
void GEngine::initEntities() {

  MGMS()->reifyPools(s_vec<sstr> {"Meteors", "Healths", "Clouds" });
  auto ent= this->reifyEntity();
  auto ss= mc_new1(GVars);
  auto wb= cx::visBox();
  ent->checkin(ss);

  MGMS()->getPool("Meteors")
  ->preset([=]() {
    auto s= cx::reifySprite("meteor.png");
    MGML()->addAtlasItem("game-pics",s, kMiddleground, kSpriteMeteor);
    return mc_new1(Meteor,s);
  }, 50);

  MGMS()->getPool("Healths")
  ->preset([=]() {
    auto s= cx::reifySprite("health.png");
    s->setAnchorPoint(c::Vec2(0.5f, 0.8f));
    MGML()->addAtlasItem("game-pics",s, kMiddleground, kSpriteHealth);
    return mc_new1(Health,s);
  }, 20);

  //CREATE BOMB
  auto sp = cx::reifySprite("bomb.png");
  sp->getTexture()->generateMipmap();
  sp->setVisible(false);
  auto sz = sp->getContentSize();
  //add sparkle
  auto sparkle = cx::reifySprite("sparkle.png");
  sparkle->setPosition(sz.width * 0.72f, sz.height * 0.72f);
  sp->addChild(sparkle, kMiddleground, kSpriteSparkle);
  //add halo
  auto halo = cx::reifySprite("halo.png");
  halo->setPosition(sz.width * 0.4f, sz.height * 0.4f);
  sp->addChild(halo, 1, kSpriteHalo);

  MGML()->addAtlasItem("game-pics",sp);
  ent= this->reifyEntity();
  ent->checkin(mc_new1(Bomb,sp));

  //add shockwave
  ss->shockWave = cx::reifySprite("shockwave.png");
  ss->shockWave->getTexture()->generateMipmap();
  ss->shockWave->setVisible(false);
  MGML()->addAtlasItem("game-pics", ss->shockWave);

  auto animation = c::Animation::create();
  //add ufo
  sp= cx::reifySprite("ufo_1.png");
  for (auto i = 1; i <= 4; ++i) {
    animation->addSpriteFrame(
        cx::getSpriteFrame(
          "ufo_" + s::to_string(i) +".png"));
  }
  animation->setDelayPerUnit(1.0 / 4.0f);
  animation->setLoops(-1);
  ss->ufoAnimation = c::Animate::create(animation);
  CC_KEEP(ss->ufoAnimation)

  auto ray = cx::reifySprite("ray.png");
  auto uz= sp->getBoundingBox().size;
  ray->setAnchorPoint(c::Vec2(0.5, 1));
  ray->setPosition(uz.width  * 0.52f, uz.height * 0.5f);

  ss->blinkRay = c::RepeatForever::create(c::Blink::create(4, 6));
  CC_KEEP(ss->blinkRay)
  sp->addChild(ray, -1, kSpriteRay);
  sp->setVisible(false);
  MGML()->addAtlasItem("game-pics", sp, 0, kSpriteUfo);

  ent=this->reifyEntity();
  ent->checkin(mc_new1(Ufo,sp));

  auto pool=MGMS()->reifyPool("Clouds");
  for (auto i = 0; i < 4; ++i) {
    auto y = wb.top * (i % 2 == 0 ? 0.4f : 0.5f);
    auto c = cx::reifySprite("cloud.png");
    c->setPosition(wb.right * 0.1f + i * wb.right * 0.3f, y);
    MGML()->addAtlasItem("game-pics", c, kBackground);
    pool->checkin(mc_new1(Cloud,c));
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

