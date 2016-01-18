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

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Stage.h"
#include "Resolve.h"
#include "Collide.h"
#include "Move.h"
#include "Aliens.h"
#include "Render.h"
#include "EFactory.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
//
static const s::array<sstr,4> BackTileMap= {
  "lvl1_map1.png", "lvl1_map2.png",
  "lvl1_map3.png", "lvl1_map4.png"
};

//////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {
  MGMS()->reifyPools(s_vec<sstr> {
      "BackTiles", "BackSkies", "Missiles", "Baddies",
      "Bombs", "Explosions", "Sparks", "HitEffects"
  });
  createBackSkies();
  createBackTiles();
  createMissiles();
  createBombs();
  createEnemies();
  createExplosions();
  createSparks();
  createHitEffects();
  createArena();
  createShip();
}

//////////////////////////////////////////////////////////////////////////
//
void GEngine::initSystems() {
  regoSystem(mc_new1(Stage, this));
  regoSystem(mc_new1(Move, this));
  regoSystem(mc_new1(Aliens, this));
  regoSystem(mc_new1(Collide, this));
  regoSystem(mc_new1(Resolve, this));
  regoSystem(mc_new1(Render, this));
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createArena() {
  auto ent= this->reifyEntity("+");
  ent->checkin(mc_new(GVars));
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createShip() {
  auto zx= CC_CSV(c::Integer, "SHIP_ZX");
  auto sp= cx::reifySprite("ship01.png");
  auto ent= this->reifyEntity("+");
  auto sz= sp->getContentSize();
  auto wz= cx::visRect();
  auto cw= cx::center();

  MGML()->addAtlasItem("game-pics", sp, zx, 911);
  auto cac= c::AnimationCache::getInstance();
  auto ani= cac->getAnimation("ShipAni");
  if (ENP(ani)) {
    c::Vector<c::SpriteFrame*> fs;
    fs.pushBack(cx::getSpriteFrame("ship01.png"));
    fs.pushBack(cx::getSpriteFrame("ship02.png"));
    cac->addAnimation(
        c::Animation::createWithSpriteFrames(fs,01.f), "ShipAni");
    ani= cac->getAnimation("ShipAni");
  }
  sp->setPosition(cw.x, sz.height);
  sp->runAction(c::RepeatForever::create( c::Animate::create(ani)));

  auto bs = cx::reifySprite("ship03.png");
  bs->setBlendFunc(BDFUNC::ADDITIVE);
  bs->setPosition(sz.width * 0.5f, 12);
  bs->setVisible(false);
  sp->addChild(bs, zx, 911);//99999);

  ent->checkin( mc_new2(Ship, sp, bs));
  ent->checkin( mc_new(Motion));
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createMissiles(int count) {
  auto zx = CC_CSV(c::Integer, "SHIP_ZX");
  auto p = MGMS()->getPool("Missiles");

  p->preset([=]() -> f::ComObj* {
    auto sp= cx::reifySprite("W1.png");
    sp->setBlendFunc(BDFUNC::ADDITIVE);
    sp->setVisible(false);
    MGML()->addAtlasItem("op-pics", sp, zx, 911);
    return mc_new1(Missile, sp);
  }, count);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createBombs(int count) {
  auto zx = CC_CSV(c::Integer, "SHIP_ZX");
  auto p = MGMS()->getPool("Bombs");

  p->preset([=]() -> f::ComObj* {
    auto sp= cx::reifySprite("W2.png");
    sp->setBlendFunc(BDFUNC::ADDITIVE);
    sp->setVisible(false);
    MGML()->addAtlasItem("op-pics", sp, zx, 911);
    return mc_new1(Bomb, sp);
  }, count);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createExplosions(int count) {
  auto zx = CC_CSV(c::Integer, "SHIP_ZX");
  auto p = MGMS()->getPool("Explosions");

  p->preset([=]() -> f::ComObj* {
    auto sp = cx::reifySprite("explosion_01.png");
    sp->setBlendFunc(BDFUNC::ADDITIVE);
    sp->setVisible(false);
    MGML()->addAtlasItem("explosions", sp);
    return mc_new1(Explosion, sp);
  }, count);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createHitEffects(int count) {
  auto zx = CC_CSV(c::Integer, "SHIP_ZX");
  auto p = MGMS()->getPool("HitEffects");

  p->preset([=]() -> f::ComObj* {
    auto sp = cx::reifySprite("hit.png");
    sp->setBlendFunc(BDFUNC::ADDITIVE);
    sp->setVisible(false);
    MGML()->addAtlasItem("op-pics", sp);
    return mc_new1(HitEffect, sp);
  }, count);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createSparks(int count) {
  auto zx = CC_CSV(c::Integer, "SHIP_ZX");
  auto p = MGMS()->getPool("Sparks");

  p->preset([=]() -> f::ComObj* {
    auto sp2 = cx::reifySprite("explode2.png");
    sp2->setBlendFunc(BDFUNC::ADDITIVE);
    sp2->setVisible(false);
    MGML()->addAtlasItem("op-pics", sp2);

    auto sp3 = cx::reifySprite("explode3.png");
    sp3->setBlendFunc(BDFUNC::ADDITIVE);
    sp3->setVisible(false);
    MGML()->addAtlasItem("op-pics", sp3);

    return mc_new2(Spark, sp2, sp3);
  }, count);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createEnemies(int count) {

  auto zx = CC_CSV(c::Integer, "SHIP_ZX") - 1;
  auto p = MGMS()->getPool("Baddies");
  auto cr= [=](const EnemyType& arg) -> f::ComObj* {
    auto sp= cx::reifySprite(arg.textureName);
    sp->setVisible(false);
    MGML()->addAtlasItem("game-pics", sp,zx,911);
    return mc_new2(Enemy, sp, arg);
  };

  F__LOOP(it, EnemyTypes) {
     auto& arg = *it;
     p->preset([=]() ->  f::ComObj* {
         return cr(arg);
     }, count);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createBackSkies() {
  auto zx = CC_CSV(c::Integer, "SHIP_ZX");
  auto p = MGMS()->getPool("BackSkies");
  auto layer= MGMS()->getLayer(1);

  p->preset([=]() -> f::ComObj* {
    auto bg = cx::reifySprite("bg01.png");
    bg->setAnchorPoint(cx::anchorBL());
    bg->setVisible(false);
    layer->addAtlasItem("game-pics", bg, -10,911);
    return mc_new1(f::ComObj, bg);
  }, 2);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createBackTiles(int count) {
  auto p = MGMS()->getPool("BackTiles");
  auto layer= MGMS()->getLayer(1);
  auto cr= [=](const sstr& name) -> f::ComObj* {
    auto sp = cx::reifySprite(name);
    sp->setAnchorPoint(cx::anchorL());
    sp->setVisible(false);
    layer->addAtlasItem("back-tiles", sp, -9,911);
    return mc_new1(f::ComObj, sp);
  };

  F__LOOP(it, BackTileMap) {
    auto& n = *it;
    p->preset([=]() -> f::ComObj* {
      return cr(n);
    }, count);
  }
}


NS_END(terra)


