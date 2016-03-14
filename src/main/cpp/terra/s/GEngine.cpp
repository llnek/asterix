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
#include "Resolve.h"
#include "Collide.h"
#include "Move.h"
#include "Aliens.h"
#include "Render.h"
#include "GEngine.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
//
static const s_arr<sstr,4> BackTileMap= {
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
  regoSystem(mc_new1(Move, this));
  regoSystem(mc_new1(Aliens, this));
  regoSystem(mc_new1(Collide, this));
  regoSystem(mc_new1(Resolve, this));
  regoSystem(mc_new1(Render, this));
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createArena() {
  auto ent= this->reifyNode(true);
  ent->checkin(mc_new(GVars));
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createShip() {
  auto ani= CC_ACAC()->getAnimation("ShipAni");
  auto bs = cx::reifySprite("ship03.png");
  auto sp= cx::reifySprite("ship01.png");
  auto zx= CC_CSV(c::Integer,"SHIP+ZX");
  auto ent= this->reifyNode(true);
  auto sz= CC_CSIZE(sp);
  auto wb= cx::visBox();

  MGML()->addAtlasItem("game-pics", sp, zx);

  // ship sprite
  sp->setPosition(wb.cx, sz.height);
  sp->runAction(
      c::RepeatForever::create(
        c::Animate::create(ani)));

  //born sprite
  bs->setBlendFunc(BDFUNC::ADDITIVE);
  bs->setPosition(HWZ(sz), 12);
  CC_HIDE(bs);
  sp->addChild(bs, zx, 911);

  ent->checkin(mc_new2(Ship, sp, bs));
  ent->checkin(mc_new(f::CGesture));
  ent->checkin(mc_new1(f::CHealth,5));
  ent->checkin(mc_new(f::CMove));
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createMissiles(int count) {
  auto speed= CC_CSV(c::Float, "MISSILE+SPEED");
  auto zx = CC_CSV(c::Integer, "SHIP+ZX");
  auto po = MGMS()->getPool("Missiles");
  po->preset([=]() -> f::Poolable* {
    auto sp= cx::reifySprite("W1.png");
    auto ui= mc_new1(Missile, sp);
    auto mv= mc_new(f::CMove);
    auto ent=this->reifyNode();

    MGML()->addAtlasItem("op-pics", sp, zx);
    sp->setBlendFunc(BDFUNC::ADDITIVE);
    CC_HIDE(sp);

    mv->speed.y= speed;
    mv->vel.y= speed;

    ent->checkin(mc_new(f::CHealth));
    ent->checkin(ui);
    ent->checkin(mv);

    return ent;

  }, count);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createBombs(int count) {
  auto speed= CC_CSV(c::Float, "BOMB+SPEED");
  auto zx = CC_CSV(c::Integer, "SHIP+ZX");
  auto po = MGMS()->getPool("Bombs");

  po->preset([=]() -> f::Poolable* {
    auto sp= cx::reifySprite("W2.png");
    auto ui= mc_new1(Bomb, sp);
    auto mv= mc_new(f::CMove);
    auto ent=this->reifyNode();

    MGML()->addAtlasItem("op-pics", sp, zx);
    sp->setBlendFunc(BDFUNC::ADDITIVE);
    CC_HIDE(sp);

    mv->speed.y= -speed;
    mv->vel.y= -speed;

    ent->checkin(mc_new(f::CHealth));
    ent->checkin(ui);
    ent->checkin(mv);
    return ent;

  }, count);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createExplosions(int count) {
  auto zx = CC_CSV(c::Integer, "SHIP+ZX");
  auto po = MGMS()->getPool("Explosions");

  po->preset([=]() -> f::Poolable* {
    auto sp = cx::reifySprite("explosion_01.png");
    auto ent= this->reifyNode();

    MGML()->addAtlasItem("explosions", sp);
    sp->setBlendFunc(BDFUNC::ADDITIVE);
    CC_HIDE(sp);

    ent->checkin(mc_new1(Explosion, sp));
    return ent;

  }, count);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createHitEffects(int count) {
  auto zx = CC_CSV(c::Integer, "SHIP+ZX");
  auto po = MGMS()->getPool("HitEffects");

  po->preset([=]() -> f::Poolable* {
    auto sp = cx::reifySprite("hit.png");
    auto ent= this->reifyNode();
    MGML()->addAtlasItem("op-pics", sp);
    sp->setBlendFunc(BDFUNC::ADDITIVE);
    CC_HIDE(sp);

    ent->checkin(mc_new1(HitEffect, sp));
    return ent;
  }, count);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createSparks(int count) {
  auto zx = CC_CSV(c::Integer, "SHIP+ZX");
  auto po = MGMS()->getPool("Sparks");

  po->preset([=]() -> f::Poolable* {
    auto sp2 = cx::reifySprite("explode2.png");
    auto sp3 = cx::reifySprite("explode3.png");
    auto ent= this->reifyNode();
    MGML()->addAtlasItem("op-pics", sp2);
    MGML()->addAtlasItem("op-pics", sp3);
    sp2->setBlendFunc(BDFUNC::ADDITIVE);
    CC_HIDE(sp2);
    sp3->setBlendFunc(BDFUNC::ADDITIVE);
    CC_HIDE(sp3);
    ent->checkin(mc_new2(Spark, sp2, sp3));
    return ent;

  }, count);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createEnemies(int count) {
  auto zx = CC_CSV(c::Integer, "SHIP+ZX") - 1;
  auto po = MGMS()->getPool("Baddies");
  auto cr= [=](const EnemyType& arg)
    -> f::Poolable* {
    auto sp= cx::reifySprite(arg.textureName);
    auto ent= this->reifyNode();
    MGML()->addAtlasItem("game-pics", sp,zx);
    CC_HIDE(sp);
    ent->checkin(mc_new1(f::CHealth,arg.HP));
    ent->checkin(mc_new1(f::CPixie, sp));
    ent->checkin(mc_new(f::CMove));
    ent->checkin(mc_new1(Enemy, arg));
    return ent;
  };

  F__LOOP(it, EnemyTypes) {
    auto &arg = *it;
    po->preset([=]() -> f::Poolable* {
      return cr(arg);
    }, count);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createBackSkies() {
  auto zx = CC_CSV(c::Integer, "SHIP+ZX");
  auto po = MGMS()->getPool("BackSkies");
  auto layer= MGMS()->getLayer(1);

  po->preset([=]() -> f::Poolable* {
    auto bg = cx::reifySprite("bg01.png");
    auto ent= this->reifyNode();
    layer->addAtlasItem("game-pics", bg, -10);
    bg->setAnchorPoint(cx::anchorBL());
    CC_HIDE(bg);
    ent->checkin(mc_new1(f::CPixie, bg));
    return ent;
  }, 2);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createBackTiles(int count) {
  auto po = MGMS()->getPool("BackTiles");
  auto layer= MGMS()->getLayer(1);
  auto cr= [=](const sstr &name) -> f::Poolable* {
    auto sp = cx::reifySprite(name);
    auto ent= this->reifyNode();
    layer->addAtlasItem("back-tiles", sp, -9);
    sp->setAnchorPoint(cx::anchorL());
    CC_HIDE(sp);
    ent->checkin(mc_new1(f::CPixie, sp));
    return ent;
  };

  F__LOOP(it, BackTileMap) {
    auto &n = *it;
    po->preset([=]() -> f::Poolable* {
      return cr(n);
    }, count);
  }
}


NS_END


