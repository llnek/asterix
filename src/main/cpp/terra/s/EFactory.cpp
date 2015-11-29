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

NS_ALIAS(terra)

static const s::array<stdstr,4> BackTileMap= {
  "lvl1_map1.png", "lvl1_map2.png",
                    "lvl1_map3.png", "lvl1_map4.png" };

//////////////////////////////////////////////////////////////////////////
//
EFactory::EFactory(not_null<a::Engine*> e,
    not_null<c::Dictionary*> options)

  : f::Factory(e, options) {

}

//////////////////////////////////////////////////////////////////////////////
//
a::Entity* EFactory::createShip() {
  auto zx = CC_CSV(c::Integer, "SHIP_ZX");
  auto sp= cx::reifySprite("ship01.png");
  auto ent= engine->reifyEntity("game");
  auto sz= sp->getContentSize();
  auto wz= cx::visRect();
  auto cw= cx::center();

  auto fr0 = cx::getSpriteFrame("ship01.png");
  auto fr1 = cx::getSpriteFrame("ship02.png");
  auto animation = c::Animation::create();
  animation->setDelayPerUnit(0.1f);
  animation->addSpriteFrame(fr0);
  animation->addSpriteFrame(fr1);
  auto animate = c::Animate::create(animation);

  MGML()->addAtlasItem("game-pics", sp, f::Maybe<int>(zx));
  sp->runAction(c::RepeatForever::create(animate));
  sp->setPosition(cw.x, sz.height);

  auto bs = cx::reifySprite("ship03.png");
  bs->setBlendFunc(c::BlendFunc::ADD);
  bs->setPosition(sz.width * 0.5f, 12);
  bs->setVisible(false);
  sp->addChild(bs, f::Maybe<int>(zx), f::Maybe<int>(99999));

  auto player = new Ship(sp, bs);
  ent->checkin(player);
  ent->checkin(new Motion());

  return ent;
}

//////////////////////////////////////////////////////////////////////////////
//
void EFactory::createMissiles(int count) {
  auto zx = CC_CSV(c::Integer, "SHIP_ZX");
  auto p = MGMS()->getPool("Missiles");

  p->preSet([=]() -> f::ComObj* {
    auto sp= cx::reifySprite("W1.png");
    sp->setBlendFunc(c::BlendFunc::ADD);
    sp->setVisible(false);
    MGML()->addAtlasItem("op-pics", sp, f::Maybe<int>(zx));
    return new Missile(sp);
  }, count);
}

//////////////////////////////////////////////////////////////////////////////
//
void EFactory::createBombs(int count) {
  auto zx = CC_CSV(c::Integer, "SHIP_ZX");
  auto p = MGMS()->getPool("Bombs");

  p->preSet([=]() -> f::ComObj* {
    auto sp= cx::reifySprite("W2.png");
    sp->setBlendFunc(c::BlendFunc::ADD);
    sp->setVisible(false);
    MGML()->addAtlasItem("op-pics", sp, f::Maybe<int>(zx));
    return new Bomb(sp);
  }, count);
}

//////////////////////////////////////////////////////////////////////////////
//
void EFactory::createExplosions(int count) {
  auto zx = CC_CSV(c::Integer, "SHIP_ZX");
  auto p = MGMS()->getPool("Explosions");

  p->preSet([=]() -> f::ComObj* {
    auto sp = cx::reifySprite("explosion_01.png");
    sp->setBlendFunc(c::BlendFunc::ADD);
    sp->setVisible(false);
    MGML()->addAtlasItem("explosions", sp);
    return new Explosion(sp);
  }, count);
}

//////////////////////////////////////////////////////////////////////////////
//
void EFactory::createHitEffects(int count) {
  auto zx = CC_CSV(c::Integer, "SHIP_ZX");
  auto p = MGMS()->getPool("HitEffects");

  p->preSet([=]() -> f::ComObj* {
    auto sp = cx::reifySprite("hit.png");
    sp->setBlendFunc(c::BlendFunc::ADD);
    sp->setVisible(false);
    MGML()->addAtlasItem("op-pics", sp);
    return new HitEffect(sp);
  }, count);
}

//////////////////////////////////////////////////////////////////////////////
//
void EFactory::createSparks(int count) {
  auto zx = CC_CSV(c::Integer, "SHIP_ZX");
  auto p = MGMS()->getPool("Sparks");

  p->preSet([=]() -> f::ComObj* {
    auto sp2 = cx::reifySprite("explode2.png");
    sp2->setBlendFunc(c::BlendFunc::ADD);
    sp2->setVisible(false);
    MGML()->addAtlasItem("op-pics", sp2);

    auto sp3 = cx::reifySprite("explode3.png");
    sp3->setBlendFunc(c::BlendFunc::ADD);
    sp3->setVisible(false);
    MGML()->addAtlasItem("op-pics", sp3);

    return new Spark(sp2, sp3);
  }, count);
}

//////////////////////////////////////////////////////////////////////////////
//
void EFactory::createEnemies(int count) {

  auto zx = CC_CSV(c::Integer, "SHIP_ZX") -1;
  auto p = MGMS()->getPool("Baddies");
  auto cr= [=](const EnemyType& arg) -> f::ComObj* {
    auto sp= cx::reifySprite(arg->textureName);
    sp->setVisible(false);
    MGMS()->addAtlasItem("game-pics", sp, f::Maybe<int>(zx));
    return new Enemy(sp, arg);
  };

  for (auto j = 0; j < EnemyTypes.size(); ++j) {
    auto& arg = EnemyTypes[j];
    p->preSet([=]() ->  f::ComObj* {
      return cr(arg);
    }, count);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void EFactory::createBackSkies() {
  auto zx = CC_CSV(c::Integer, "SHIP_ZX");
  auto p = MGMS()->getPool("BackSkies");
  auto layer= MGMS()->getLayer(1);

  p->preSet([=]() -> f::ComObj* {
    auto bg = cx::reifySprite("bg01.png");
    bg->setAnchorPoint(cx::anchorBL());
    bg->setVisible(false);
    layer->addAtlasItem("game-pics", bg, f::Maybe<int>(-10));
    return new f::ComObj(bg);
  }, 2);
}

//////////////////////////////////////////////////////////////////////////////
//
void EFactory::createBackTiles(int count) {
  auto layer= MGMS()->getLayer(1);
  auto cr= [=](const stdstr& name) -> f::ComObj* {
    auto sp = cx::reifySprite(name);
    sp->setAnchorPoint(cx::anchorL());
    sp->setVisible(false);
    layer->addAtlasItem("back-tiles", sp, f::Maybe<int>(-9));
    return new f::ComObj(sp);
  };

  for (auto j=0; j < BackTileMap.size(); ++j) {
    auto& n = BackTileMap[j];
    p->preSet([=]() -> f::ComObj* {
      return cr(n);
    }, count);
  }
}




NS_ALIAS(terra)


