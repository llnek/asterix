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

#include "core/XConfig.h"
#include "core/CCSX.h"
#include "core/XPool.h"
#include "x2d/XGameLayer.h"
#include "x2d/MainGame.h"
#include "ash/Engine.h"
#include "n/gnodes.h"
#include "Factory.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
Factory::Factory(not_null<a::Engine*> e, not_null<c::Dictionary*> options) {
  state= options;
  engine = e;
  state->retain();
}

//////////////////////////////////////////////////////////////////////////
//
Factory::~Factory() {
  if (NNP(state)) { state->release(); }
}

//////////////////////////////////////////////////////////////////////////
//
Factory::Factory() {
  SNPTR(engine)
  SNPTR(state)
}

//////////////////////////////////////////////////////////////////////////
//
void Factory::CreateMissiles(int count) {
  auto p= MGMS()->GetPool("missiles");
  auto cb= []() -> f::ComObj* {
    auto sp = cx::CreateSprite("missile.png");
    sp->setVisible(false);
    MGML()->AddAtlasItem("game-pics", sp);
    return new Missile(sp);
  };
  p->Preset(cb, count);
}

//////////////////////////////////////////////////////////////////////////
//
void Factory::CreateExplosions(int count) {
  auto p= MGMS()->GetPool("explosions");
  auto cb= []() -> f::ComObj* {
    auto sp = cx::CreateSprite("boom_0.png");
    sp->setVisible(false);
    MGML()->AddAtlasItem("game-pics", sp);
    return new Explosion(sp);
  };
  p->Preset(cb, count);
}

//////////////////////////////////////////////////////////////////////////
//
void Factory::CreateBombs(int count) {
  auto p= MGMS()->GetPool("bombs");
  auto cb= []() -> f::ComObj* {
    auto sp = cx::CreateSprite("bomb.png");
    sp->setVisible(false);
    MGML()->AddAtlasItem("game-pics", sp);
    return new Bomb(sp);
  };
  p->Preset(cb, count);
}

//////////////////////////////////////////////////////////////////////////
//
const c::Size Factory::CalcImgSize(const stdstr& img) {
  return cx::CalcSize(img);
}

//////////////////////////////////////////////////////////////////////////
//
c::Dictionary* Factory::GetRankInfo(int r) {
  c::Size z= cx::CalcSize("purple_bug_0.png");
  stdstr s0 = "purple_bug_0.png";
  stdstr s1= "purple_bug_1.png";
  int v= 30;
  auto d = c::Dictionary::create();

  if (r < 3) {
    v= 100;
    s0 = "blue_bug_0.png";
    s1 = "blue_bug_1.png";
    z= CalcImgSize("blue_bug_0.png");
  }

  if (r < 5) {
    v= 50;
    s0 = "green_bug_0.png";
    s1= "green_bug_1.png";
    z = CalcImgSize("green_bug_0.png");
  }

  d->setObject(f::Size2::create(z.width, z.height), "size");
  d->setObject(c::Integer::create(v), "value");
  d->setObject(c::String::create(s0), "img0");
  d->setObject(c::String::create(s1), "img1");

  return d;
}

//////////////////////////////////////////////////////////////////////////
//
void Factory::FillSquad(not_null<f::XPool*> pool) {

  auto cells = f::CstVal<c::Integer>("CELLS")->getValue();
  auto cols = f::CstVal<c::Integer>("COLS")->getValue();
  auto wz= cx::VisRect();
  auto wb= cx::VisBox();

  auto cache= c::AnimationCache::getInstance();
  auto info= GetRankInfo(0);
  float x;
  float y;
  int row=0;

  for (int n=0; n < cells; ++n) {

    auto az= f::DictVal<f::Size2>(info, "size")->getValue();
    if (n % cols == 0) {
      y = (n == 0) ? wb.top * 0.9
                   : y - az.height - wz.size.height * 4/480;
      x = wb.left + (8/320 * wz.size.width) + HWZ(az);
      row += 1;
      info= GetRankInfo(row);
      az= f::DictVal<f::Size2>(info, "size")->getValue();
    }

    auto s = f::DictVal<c::String>(info, "img0");
    auto aa = cx::CreateSprite(s->getCString());
    aa->setPosition(x + HWZ(az), y - HHZ(az));

    auto f1= f::DictVal<c::String>(info, "img0")->getCString();
    auto f2= f::DictVal<c::String>(info, "img1")->getCString();
    c::Vector<c::SpriteFrame*> animFrames(2);

    animFrames.pushBack( cx::GetSpriteFrame(f1));
    animFrames.pushBack( cx::GetSpriteFrame(f2));

    aa->runAction(c::RepeatForever::create(
      c::Animate::create(
        c::Animation::createWithSpriteFrames( animFrames, 1))));

    MGML()->AddAtlasItem("game-pics", aa);
    x += az.width + (8/320 * wz.size.width);
    auto v = f::DictVal<c::Integer>(info, "value")->getValue();
    auto co= new Alien(aa, v, row);
    co->status=true;
    pool->Checkin(co);
  }
}

//////////////////////////////////////////////////////////////////////////
//
a::Entity* Factory::CreateAliens() {
  auto stepx= f::DictVal<f::Size2>(state, "alienSize")->getValue().width /3;
  auto ent= engine->CreateEntity("baddies");
  auto p = MGMS()->GetPool("aliens");

  FillSquad(p);

  ent->Rego(new AlienSquad(p, stepx));
  ent->Rego(new Looper());

  return ent;
}

//////////////////////////////////////////////////////////////////////////
//
void Factory::BornShip() {
  CCASSERT(player != nullptr, "player cannot be null");
  player->Inflate();
}

//////////////////////////////////////////////////////////////////////////
//
a::Entity* Factory::CreateShip() {

  auto sz = f::DictVal<f::Size2>(state, "shipSize")->getValue();
  auto ent= engine->CreateEntity("goodies");
  auto s= cx::CreateSprite("ship_1.png");
  auto wz= cx::VisRect();
  auto wb= cx::VisBox();
  auto y = sz.height + wb.bottom + (5/60 * wz.size.height);
  auto x = wb.left + wz.size.width * 0.5;
  auto ship = new Ship(s, "ship_1.png", "ship_0.png");

  CCASSERT(s != nullptr, "ship sprite cannot be null");

  MGML()->AddAtlasItem("game-pics", s);
  ship->Inflate(x,y);
  player= ship;

  ent->Rego(new Velocity(150,0));
  ent->Rego(new Looper());
  ent->Rego(new Cannon(0.3));
  ent->Rego(new Motion());

  ent->Rego(ship);

  return ent;
}



NS_END(invaders)


