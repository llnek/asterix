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

#include "Factory.h"
NS_USING(ash)
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////
//
Factory::Factory(Engine* e, options)
  : engine(e),
  state(options) {
}

//////////////////////////////////////////////////////////////////////////
//
void Factory::CreateMissiles(int count) {
  auto p=XConfig::GetInstance()->GetPool("missiles");
  auto cb= []() -> Component* {
    auto sp = CCSX::CreateSprite("missile.png");
    sp->setVisible(false);
    MainGame::Get()->AddAtlasItem("game-pics", sp);
    return new Missile(sp);
  };
  p->PreSet(cb, count);
}

//////////////////////////////////////////////////////////////////////////
//
void Factory::CreateExplosions(int count) {
  auto p=XConfig::GetInstance()->GetPool("explosions");
  auto cb= []() -> Component* {
    const sp = CCSX::CreateSprite("boom_0.png");
    sp->setVisible(false);
    MainGame::Get()->AddAtlasItem("game-pics", sp);
    return new Explosion(sp);
  };
  p->PreSet(cb, count);
}

//////////////////////////////////////////////////////////////////////////
//
void Factory::CreateBombs(int count) {
  auto p=XConfig::GetInstance()->GetPool("bombs");
  auto cb= []() -> Component* {
    const sp = CCSX::CreateSprite("bomb.png");
    sp->setVisible(false);
    MainGame::Get()->AddAtlasItem("game-pics", sp);
    return new Bomb(sp);
  };
  p->PreSet(cb, count);
}

//////////////////////////////////////////////////////////////////////////
//
const Size Factory::CalcImgSize(const string& img) {
  return CCSX::CSize(img);
}

//////////////////////////////////////////////////////////////////////////
//
Dictionary* Factory::GetRankInfo(const Rank r) {
  auto d = Directory::create();
  int v= 30;
  Size z= CalcImgSize("purple_bug_0.png");
  string s0 = "purple_bug_0.png";
  string s1= "purple_bug_1.png";

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

  d->setObject(Size2::create(z.width, z.height), "size");
  d->setObject(Integer::create(v), "value");
  d->setObject(String::create(s0), "img0");
  d->setObject(String::create(s1), "img1");

  return d;
}

//////////////////////////////////////////////////////////////////////////
//
void Factory::FillSquad(XPool* pool) {
  auto cfg = XConfig::GetInstance();
  auto info= GetRankInfo(0);
  auto wz= CCSX::VisRect();
  auto wb= CCSX::VisBox();
  int cells = SCAST(cfg->GetCst("CELLS"),Integer*)->getValue();
  int cols = SCAST(cfg->GetCst("COLS"),Integer*)->getValue();
  int v, row;
  Size az;
  Sprite* aa,
  float x,y;

  az= DictVal<Size2>(info, "size")->getValue();
  row=0;
  for (int n=0; n < cells; ++n) {
    if (n % cols == 0) {
      y = (n == 0) ? wb.top * 0.9
                   : y - az.height - wz.height * 4/480;
      x = wb.left + (8/320 * wz.width) + HWZ(az);
      row += 1;
      info= GetRankInfo(row);
      az= DictVal<Size2>(info, "size")->getValue();
    }
    aa = DictVal<String>(info, "img0")->getCString();
    aa->setPosition(x + HWZ(az), y - HHZ(az));
    aa->runAction(RepeatForever::create(
      Animate::create(Animation::create(
          [ccsx.getSprite(info[1][0]),
           ccsx.getSprite(info[1][1]) ], 1))));
    MainGame::Get()->AddAtlasItem("game-pics", aa);
    x += az.width + (8/320 * wz.width);
    v= DictVal<Integer>(info, "value")->getValue();
    aa= new Alien(aa, v, row);
    aa->status=true;
    pool->Add(aa);
  }
}

//////////////////////////////////////////////////////////////////////////
//
Entity* Factory::CreateAliens() {
  auto stepx= DictVal<Size2>(state, "alienSize")->getValue().width /3;
  auto cfg = XConfig::GetInstance();
  auto ent= engine->CreateEntity("baddies");
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
  auto d = Dictionary::create();
  if (NNP(s)) {
    s->Inflate(d);
  }
}

//////////////////////////////////////////////////////////////////////////
//
Entity* Factory::CreateShip() {
  auto s= CCSX::CreateSprite("ship_1.png");
  auto ent= engine->CreateEntity("goodies");
  auto wz= CCSX::VisRect();
  auto wb= CCSX::VisBox();
  auto sz = DictVal<Size2>(state, "shipSize")->getValue();
  auto y = sz.height + wb.bottom + (5/60 * wz.height);
  auto x = wb.left + wz.width * 0.5;
  auto options = Dictionary::create();
  Ship* ship;

  MainGame::Get()->AddAtlasItem("game-pics", s);
  options->setObject(Float::create(x), "x");
  options->setObject(Float::create(y), "y");
  ship = new Ship(s, "ship_1.png", "ship_0.png");
  ship->Inflate(options);
  ent->Add(ship);

  ent->Add(new Velocity(150,0));
  ent->Add(new Looper(1));
  ent->Add(new Cannon());
  ent->Add(new Motion());

  return ent;
}



NS_END(invaders)
