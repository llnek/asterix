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
  auto ss= CC_GEC(GVars, _arena,"n/GVars");
  auto ens= MGMS()->getPool("Baddies");
  auto json= MGMS()->getLCfg();
  auto cfg= json->getValue();
  auto dirty=false;
  auto dt= ss->secCount;

  if (ens->countActives() < JS_INT(cfg["enemyMax"])) {
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
  auto ui= CC_GEC(f::CPixie, enemy, "f/CPixie");
  auto et= CC_GEC(Enemy, enemy, "n/Enemy");
  auto bombs= MGMS()->getPool("Bombs");
  auto b = (ecs::Node*)bombs->take(true);
  auto pos= ui->getPosition();
  auto sz= CC_CSIZE(ui);
  auto bs= CC_GEC(Missile, b, "f/CPixie");

  bs->setAttackMode( et->enemyType.attackMode);
  cx::resurrect(b, pos.x, pos.y - sz.height * 0.2);
}

//////////////////////////////////////////////////////////////////////////
//
ecs::Node* Aliens::getB(const EnemyType &arg) {
  auto ens = MGMS()->getPool("Baddies");
  auto pred= [=](f::Poolable *p) -> bool {
    auto et= CC_GEC(Enemy, p, "n/Enemy");
    return (et->enemyType.type == arg.type &&
            p->status() == false);
  };

  auto en= ens->select(pred);
  ecs::Node *y= CC_NIL;

  if (E_NIL(en)) {
    SCAST(GEngine*, _engine)->createEnemies(1);
    en= ens->select(pred);
  }

  y = PCAST(ecs::Node,en);
  if (y) {
    auto ui= CC_GEC(f::CPixie, y, "f/CPixie");
    auto et= CC_GEC(Enemy, y, "n/Enemy");
    ui->schedule([=](float) {
      this->dropBombs(y);
    }, et->delayTime, "dropBombs");
    cx::resurrect(y);
  }

  return y;
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::addEnemyToGame(int enemyType) {
  auto sp= CC_GEC(Ship, _ship, "f/CPixie");
  auto &arg = EnemyTypes()[enemyType];
  auto wz = cx::visSize();
  auto en = getB(arg);
  if (! en) { return; }
  auto ens= CC_GEC(f::CPixie, en, "f/CPixie");
  auto et= CC_GEC(Enemy, en, "n/Enemy");
  auto sz= CC_CSIZE(ens);
  auto epos= ens->getPosition();
  auto pos= sp->getPosition();
  c::Action *act;

  CC_POS2(ens,cx::randInt(HWZ(wz) + 80), wz.height);

  switch (et->enemyType.moveType) {
    case Moves::RUSH:
      act = c::MoveTo::create(1, CCT_PT(pos.x, pos.y));
    break;

    case Moves::VERT:
      act = c::MoveBy::create(4, CCT_PT(0, -wz.height - sz.height));
    break;

    case Moves::HORZ: {
      auto a0 = c::MoveBy::create(0.5, CCT_PT(0, -100 - cx::randInt(200)));
      auto a1 = c::MoveBy::create(1, CCT_PT(-50 - cx::randInt(100), 0));
      auto cb = [=]() {
        auto a2 = c::DelayTime::create(1);
        auto a3 = c::MoveBy::create(1, CCT_PT(100 + cx::randInt(100), 0));
        ens->runAction(
            c::RepeatForever::create(
              c::Sequence::create(a2, a3,
                a2->clone(), a3->reverse(), CC_NIL)));
      };
      act = c::Sequence::create(
          a0, a1, c::CallFunc::create(cb), CC_NIL);
    }
    break;

    case Moves::OLAP: {
      auto newX = pos.x <= HWZ(wz) ? wz.width : -wz.width;
      auto a4 = c::MoveBy::create(4, CCT_PT(newX, -wz.width * 0.75));
      auto a5 = c::MoveBy::create(4, CCT_PT(-newX, -wz.width));
      act = c::Sequence::create(a4,a5, CC_NIL);
    }
    break;
  }

  ens->runAction(act);
}


NS_END


