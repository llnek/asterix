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
#include "core/JSON.h"
#include "core/CCSX.h"
#include "Aliens.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////
//
void Aliens::preamble() {
  _ship = _engine->getNodes("f/CGesture")[0];
  _arena = _engine->getNodes("n/GVars")[0];
}

//////////////////////////////////////////////////////////////////////////
//
bool Aliens::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::process(float) {
  auto enemies= MGMS()->getPool("Baddies");
  auto ss= CC_GEC(GVars, _arena,"n/GVars");
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
void Aliens::dropBombs(ecs::Node *enemy) {
  auto ui= CC_GEC(f::CDraw, enemy, "f/CDraw");
  auto et= CC_GEC(Enemy, enemy, "n/Enemy");
  auto bombs= MGMS()->getPool("Bombs");
  auto sz= ui->csize();
  auto pos= ui->pos();
  auto b = (ecs::Node*) bombs->getAndSet(true);
  auto bs= CC_GEC(f::CDraw, b, "f/CDraw");
  SCAST(Bomb*,b)->attackMode= et->attackMode;
  bs->inflate(pos.x, pos.y - sz.height * 0.2);
}

//////////////////////////////////////////////////////////////////////////
//
ecs::Node* Aliens::getB(const EnemyType &arg) {
  auto enemies = MGMS()->getPool("Baddies");
  auto pred= [=](f::Poolable *p) -> bool {
    auto e = (ecs::Node*)p;
    auto ui= CC_GEC(f::CDraw, e, "f/CDraw");
    auto et= CC_GEC(Enemy, e, "n/Enemy");
    return (et->enemyType.type == arg.type &&
            e->status() == false);
  };

  auto en= enemies->select(pred);
  ecs::Node *y= CC_NIL;

  if (ENP(en)) {
    SCAST(GEngine*,engine)->createEnemies(1);
    en= enemies->select(pred);
  }

  if (NNP(en)) {
    y = (ecs::Node*) en;
    auto h= CC_GEC(f::CHealth, y, "f/CHealth");
    auto ui= CC_GEC(f::CDraw, y, "f/CDraw");
    auto et= CC_GEC(Enemy, y, "n/Enemy");
    ui->node->schedule([=](float) {
      this->dropBombs(y);
    }, et->delayTime, "dropBombs");
    ui->inflate();
    h->reset();
    y->take();
  }

  return y;
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::addEnemyToGame(int enemyType) {
  auto sp= CC_GEC(Ship, _ship, "f/CDraw");
  auto &arg = EnemyTypes[enemyType];
  auto wz = cx::visRect();
  auto en = getB(arg);
  if (ENP(en)) { return; }
  auto ens= CC_GEC(f::CDraw, en, "f/CDraw");
  auto et= CC_GEC(Enemy, en, "n/Enemy");
  auto pos= sp->pos();
  auto sz= ens->csize();
  auto epos= ens->pos();
  c::Action *act;

  ens->setPos(cx::randFloat(HWZ(wz.size) + 80.0), wz.size.height);
  switch (et->enemyType.moveType) {
    case Moves::RUSH:
      act = c::MoveTo::create(1, c::Vec2(pos.x, pos.y));
    break;

    case Moves::VERT:
      act = c::MoveBy::create(4, c::Vec2(0, -wz.size.height - sz.height));
    break;

    case Moves::HORZ: {
      auto a0 = c::MoveBy::create(0.5, c::Vec2(0, -100 - cx::randInt(200)));
      auto a1 = c::MoveBy::create(1, c::Vec2(-50 - cx::randInt(100), 0));
      auto cb = [=]() {
        auto a2 = c::DelayTime::create(1);
        auto a3 = c::MoveBy::create(1, c::Vec2(100 + cx::randInt(100), 0));
        ens->node->runAction(
            c::RepeatForever::create(
              c::Sequence::create(a2, a3,
                a2->clone(), a3->reverse(), CC_NIL)));
      };
      act = c::Sequence::create(
          a0, a1, c::CallFunc::create(cb), CC_NIL);
    }
    break;

    case Moves::OLAP: {
      auto newX = pos.x <= HWZ(wz.size) ? wz.size.width : -wz.size.width;
      auto a4 = c::MoveBy::create(4, c::Vec2(newX, -wz.size.width * 0.75));
      auto a5 = c::MoveBy::create(4, c::Vec2(-newX, -wz.size.width));
      act = c::Sequence::create(a4,a5, CC_NIL);
    }
    break;
  }

  ens->node->runAction(act);
}


NS_END


