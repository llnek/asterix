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

#include "core/CCSX.h"
#include "Aliens.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)


//////////////////////////////////////////////////////////////////////////////
//
Aliens::Aliens(not_null<a::Engine*> e, not_null<c::Dictionary*> d)

  : BaseSystem<EFactory>(e, d) {

  SNPTR(ships)
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::addToEngine(not_null<a::Engine*> e) {
  ShipMotionNode n;
  ships = e->getNodeList(n.typeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Aliens::onUpdate(float dt) {
  auto cnt= CC_GDV(c::Float, options, "secCount");
  auto node = ships->head;
  if (NNP(node)) {
    doIt(node, cnt);
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::doIt(a::Node* node, float dt) {
  auto enemies= MGMS()->getPool("Baddies");
  auto cfg= MGMS()->getLCfg()->getValue();

  if (enemies->countActives() <
      cfg["enemyMax"].get<j::json::number_integer_t>()) {

    auto arr= cfg["enemies"].get<j::json::array_t>();
    J__LOOP(it, arr) {
      auto& a = *it;
      auto time = a["time"].get<j::json::number_integer_t>();
      auto style = a["style"].get<j::json::string_t>();
      auto types = a["types"].get<j::json::array_t>();
      auto fc = [=]() {
        J__LOOP(t,types) {
          auto& v = *t;
          this->addEnemyToGame(node,
              v.get<j::json::number_integer_t>());
        }
      };
      if (style == "*" &&
          dt % time == 0) {
        fc();
      }
      else
      if (style == "1" &&
          time >= dt) {
        style= "0";
        fc();
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::dropBombs(Enemy* enemy) {
  auto bombs= MGMS()->getPool("Bombs");
  auto sp= enemy->sprite;
  auto sz= sp->getContentSize();
  auto pos= sp->getPosition();
  Bomb* b = (Bomb*)bombs->get();

  if (ENP(b)) {
    factory->createBombs();
    b= (Bomb*)bombs->get();
  }

  b->inflate(pos.x, pos.y - sz.height * 0.2f);
  b->attackMode= enemy->attackMode;
}

//////////////////////////////////////////////////////////////////////////
//
Enemy* Aliens::getB(const EnemyType& arg) {
  auto enemies = MGMS()->getPool("Baddies");
  auto pred= [=](f::ComObj* c) -> bool {
    auto e = (Enemy*)c;
    return (e->enemyType == arg.type &&
            e->status == false);
  };

  auto en= enemies->select(pred);
  Enemy* y= nullptr;

  if (ENP(en)) {
    factory->createEnemies(1);
    en= enemies->select(pred);
  }

  if (NNP(en)) {
    y = (Enemy*) en;
    y->sprite->schedule([=]() {
      this->dropBombs(y);
    }, y->delayTime);
    y->inflate();
  }

  return y;
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::addEnemyToGame(a::Node* node, int enemyType) {
  auto& arg = EnemyTypes[enemyType];
  auto wz = cx::visRect();
  auto en = getB(arg);

  if (ENP(en)) { return; }

  auto ship= CC_GNF(Ship, node, "ship");
  auto sprite= en->sprite;
  auto pos= ship->pos();
  auto sz= en->size();
  auto epos= en->pos();
  c::Action* act;

  en->setPos(cx::randFloat(wz.width *0.5f + 80.0f), wz.height);
  switch (en->moveType) {

    case Moves::RUSH:
      act = c::MoveTo::create(1, ccp(pos.x, pos.y));
    break;

    case Moves::VERT:
      act = c::MoveBy::create(4, ccp(0, -wz.height - sz.height));
    break;

    case Moves::HORZ:
      auto a0 = c::MoveBy::create(0.5f, ccp(0, -100 - cx::randInt(200)));
      auto a1 = c::MoveBy::create(1, ccp(-50 - cx::randInt(100), 0));
      auto cb = [=]() {
        auto a2 = c::DelayTime::create(1);
        auto a3 = c::MoveBy::create(1, ccp(100 + cx::randInt(100), 0));
        sprite->runAction(c::RepeatForever::create(
              c::Sequence::create(a2, a3,
                                a2->clone(),
                                a3->reverse())));
      };
      act = c::Sequence::create(a0, a1, cc::CallFunc::create(cb));
    break;

    case Moves::OLAP:
      auto newX = (pos.x <= wz.width * 0.5f) ? wz.width : -wz.width;
      auto a0 = c::MoveBy::create(4, ccp(newX, -wz.width * 0.75f));
      auto a1 = c::MoveBy::create(4, ccp(-newX, -wz.width));
      act = c::Sequence::create(a0,a1);
    break;
  }

  sprite->runAction(act);
}


NS_END(terra)


