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

#include "support/XConfig.h"
#include "support/CCSX.h"
#include "support/XPool.h"
#include "2d/MainGame.h"
#include "ash/Engine.h"
#include "Factory.h"
NS_ALIAS(cx, fusilli::ccsx)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
Factory::Factory(f::Engine* e, cc::Dictionary* options)
  : engine(e)
  , state(options) {
  options->retain();
}

//////////////////////////////////////////////////////////////////////////
//
Factory::~Factory() {
  state->release();
}

//////////////////////////////////////////////////////////////////////////
//
Factory::Factory() {
  engine=nullptr;
  state=nullptr;
}

//////////////////////////////////////////////////////////////////////////
//
void Factory::CreateMissiles(int count) {
  auto p= f::XConfig::GetInstance()->GetPool("missiles");
  auto cb= []() -> a::Component* {
    auto sp = cx::CreateSprite("missile.png");
    sp->setVisible(false);
    MainGame::Get()->AddAtlasItem("game-pics", sp);
    return new Missile(sp);
  };
  p->PreSet(cb, count);
}

//////////////////////////////////////////////////////////////////////////
//
void Factory::CreateExplosions(int count) {
  auto p= f::XConfig::GetInstance()->GetPool("explosions");
  auto cb= []() -> a::Component* {
    const sp = cx::CreateSprite("boom_0.png");
    sp->setVisible(false);
    MainGame::Get()->AddAtlasItem("game-pics", sp);
    return new Explosion(sp);
  };
  p->PreSet(cb, count);
}

//////////////////////////////////////////////////////////////////////////
//
void Factory::CreateBombs(int count) {
  auto p= f::XConfig::GetInstance()->GetPool("bombs");
  auto cb= []() -> a::Component* {
    const sp = cx::CreateSprite("bomb.png");
    sp->setVisible(false);
    MainGame::Get()->AddAtlasItem("game-pics", sp);
    return new Bomb(sp);
  };
  p->PreSet(cb, count);
}

//////////////////////////////////////////////////////////////////////////
//
const cc::Size Factory::CalcImgSize(const s::string& img) {
  return cx::CSize(img);
}

//////////////////////////////////////////////////////////////////////////
//
cc::Dictionary* Factory::GetRankInfo(const Rank r) {
  cc::Size z= CalcImgSize("purple_bug_0.png");
  s::string s0 = "purple_bug_0.png";
  s::string s1= "purple_bug_1.png";
  int v= 30;
  auto d = cc::Directory::create();

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
  d->setObject(cc::Integer::create(v), "value");
  d->setObject(cc::String::create(s0), "img0");
  d->setObject(cc::String::create(s1), "img1");

  return d;
}

//////////////////////////////////////////////////////////////////////////
//
void Factory::FillSquad(f::XPool* pool) {

  int cells = CstVal<cc::Integer>("CELLS")->getValue();
  int cols = CstVal<cc::Integer>("COLS")->getValue();
  auto cfg = f::XConfig::GetInstance();
  auto info= GetRankInfo(0);
  auto wz= cx::VisRect();
  auto wb= cx::VisBox();

  cc::Sprite* aa;
  cc::Size az;
  int v, row;
  float x,y;

  az= DictVal<f::Size2>(info, "size")->getValue();
  row=0;
  for (int n=0; n < cells; ++n) {
    if (n % cols == 0) {
      y = (n == 0) ? wb.top * 0.9
                   : y - az.height - wz.height * 4/480;
      x = wb.left + (8/320 * wz.width) + HWZ(az);
      row += 1;
      info= GetRankInfo(row);
      az= DictVal<f::Size2>(info, "size")->getValue();
    }
    aa = DictVal<cc::String>(info, "img0")->getCString();
    aa->setPosition(x + HWZ(az), y - HHZ(az));

    Vector<SpriteFrame*> animFrames(2);
    auto f1 = DictVal<cc::String>(info, "img0")->getCString();
    auto f2 = DictVal<cc::String>(info, "img1")->getCString();
    animFrames.pushBack( cx::CreateSprite(f1));
    animFrames.pushBack( cx::CreateSprite(f2));

    aa->runAction(cc::RepeatForever::create(
      cc::Animate::create(
        cc::Animation::createWithSpriteFrames( animFrames, 1))));

    MainGame::Get()->AddAtlasItem("game-pics", aa);
    x += az.width + (8/320 * wz.width);
    v= DictVal<cc::Integer>(info, "value")->getValue();
    aa= new Alien(aa, v, row);
    aa->status=true;
    pool->Add(aa);
  }
}

//////////////////////////////////////////////////////////////////////////
//
Entity* Factory::CreateAliens() {
  auto stepx= DictVal<f::Size2>(state, "alienSize")->getValue().width /3;
  auto ent= engine->CreateEntity("baddies");
  auto cfg = f::XConfig::GetInstance();
  auto p = cfg->GetPool("aliens");

  FillSquad(p);

  ent->Add(new AlienSquad(p, stepx));
  ent->Add(new Looper(2));

  return ent;
}

//////////////////////////////////////////////////////////////////////////
//
void Factory::BornShip() {
  auto s= MainGame::Get()->GetPlayer();
  auto d = cc::Dictionary::create();

  if (NNP(s)) {
    s->Inflate(d);
  }
}

//////////////////////////////////////////////////////////////////////////
//
a::Entity* Factory::CreateShip() {

  auto sz = DictVal<f::Size2>(state, "shipSize")->getValue();
  auto ent= engine->CreateEntity("goodies");
  auto s= cx::CreateSprite("ship_1.png");
  auto wz= cx::VisRect();
  auto wb= cx::VisBox();
  auto y = sz.height + wb.bottom + (5/60 * wz.height);
  auto x = wb.left + wz.width * 0.5;
  auto options = cc::Dictionary::create();
  Ship* ship;

  f::MainGame::Get()->AddAtlasItem("game-pics", s);

  options->setObject(cc::Float::create(x), "x");
  options->setObject(cc::Float::create(y), "y");

  ship = new Ship(s, "ship_1.png", "ship_0.png");
  ship->Inflate(options);

  ent->Add(new Velocity(150,0));
  ent->Add(new Looper(1));
  ent->Add(new Cannon());
  ent->Add(new Motion());

  ent->Add(ship);

  return ent;
}



NS_END(invaders)


