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
#include "core/JSON.h"
#include "core/CCSX.h"
#include "Aliens.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////
//
void Aliens::preamble() {
  arena = engine->getNodeList(ArenaNode().typeId());
  ship = engine->getNodeList(ShipNode().typeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Aliens::update(float dt) {
  if (MGMS()->isLive()) {
    doIt(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::doIt(float) {
  auto enemies= MGMS()->getPool("Baddies");
  auto ss= CC_GNLF(GVars,arena,"slots");
  auto json= MGMS()->getLCfg();
  auto cfg= json->getValue();
  auto dirty=false;
  auto dt= ss->secCount;

  if (enemies->countActives() < JS_INT(cfg["enemyMax"])) {
    j::json arr= JS_ARR(cfg["enemies"]);
    J__LOOP(it, arr) {
      auto &a = *it;
      j::json types =  JS_ARR(a["types"]);
      auto style = JS_STR(a["style"]);
      auto time = JS_INT(a["time"]);
      if (style == "*" &&
          f::modulo(dt, time) == 0) {
        addEnemy(types);
      }
      else
      if (style == "1" &&
          time >= dt) {
        a["style"] = "0";
        dirty=true;
        addEnemy(types);
      }
    }
    if (dirty) {
      cfg["enemies"] = arr;
      json->setValue(cfg);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::addEnemy(j::json &obj) {
  J__LOOP(t, obj) {
    auto &v = *t;
    addEnemyToGame(JS_INT(v));
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::dropBombs(Enemy *enemy) {
  auto bombs= MGMS()->getPool("Bombs");
  auto sz= enemy->csize();
  auto pos= enemy->pos();
  auto b = bombs->get();

  if (ENP(b)) {
    SCAST(GEngine*,engine)->createBombs();
    b= bombs->get();
  }

  SCAST(Bomb*,b)->attackMode= enemy->enemyType.attackMode;
  b->inflate(pos.x, pos.y - sz.height * 0.2f);
}

//////////////////////////////////////////////////////////////////////////
//
Enemy* Aliens::getB(const EnemyType &arg) {
  auto enemies = MGMS()->getPool("Baddies");
  auto pred= [=](f::ComObj* c) -> bool {
    auto e = (Enemy*)c;
    return (e->enemyType.type == arg.type &&
            e->status == false);
  };

  auto en= enemies->select(pred);
  Enemy *y= nullptr;

  if (ENP(en)) {
    SCAST(GEngine*,engine)->createEnemies(1);
    en= enemies->select(pred);
  }

  if (NNP(en)) {
    y = (Enemy*) en;
    y->sprite->schedule([=](float) {
      this->dropBombs(y);
    }, y->delayTime, "dropBombs");
    y->inflate();
  }

  return y;
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::addEnemyToGame(int enemyType) {
  auto sp= CC_GNLF(Ship, ship, "ship");
  auto &arg = EnemyTypes[enemyType];
  auto wz = cx::visRect();
  auto en = getB(arg);

  if (ENP(en)) { return; }

  auto sprite= en->sprite;
  auto pos= sp->pos();
  auto sz= en->csize();
  auto epos= en->pos();
  c::Action *act;

  en->setPos(cx::randFloat(wz.size.width *0.5f + 80.0f), wz.size.height);
  switch (en->enemyType.moveType) {

    case Moves::RUSH: {
      act = c::MoveTo::create(1, c::ccp(pos.x, pos.y));
    }
    break;

    case Moves::VERT: {
      act = c::MoveBy::create(4, c::ccp(0, -wz.size.height - sz.height));
    }
    break;

    case Moves::HORZ: {
      auto a0 = c::MoveBy::create(0.5f, c::ccp(0, -100 - cx::randInt(200)));
      auto a1 = c::MoveBy::create(1, c::ccp(-50 - cx::randInt(100), 0));
      auto cb = [=]() {
        auto a2 = c::DelayTime::create(1);
        auto a3 = c::MoveBy::create(1, c::ccp(100 + cx::randInt(100), 0));
        sprite->runAction(c::RepeatForever::create(
              c::Sequence::create(a2, a3,
                a2->clone(), a3->reverse(), nullptr)));
      };
      act = c::Sequence::create(a0, a1, c::CallFunc::create(cb), nullptr);
    }
    break;

    case Moves::OLAP: {
      auto newX = (pos.x <= wz.size.width * 0.5f) ? wz.size.width : -wz.size.width;
      auto a4 = c::MoveBy::create(4, c::ccp(newX, -wz.size.width * 0.75f));
      auto a5 = c::MoveBy::create(4, c::ccp(-newX, -wz.size.width));
      act = c::Sequence::create(a4,a5,nullptr);
    }
    break;
  }

  sprite->runAction(act);
}


NS_END(terra)


