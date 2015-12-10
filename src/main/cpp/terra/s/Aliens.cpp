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
#include "ash/Node.h"
#include "core/JSON.h"
#include "core/CCSX.h"
#include "Aliens.h"
#include "n/GNodes.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(terra)


//////////////////////////////////////////////////////////////////////////////
//
Aliens::Aliens(not_null<EFactory*> e, not_null<c::Dictionary*> d)

  : BaseSystem<EFactory>(e, d) {

  SNPTR(ships)
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::addToEngine(not_null<a::Engine*> e) {
  ShipNode n;
  ships = e->getNodeList(n.typeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Aliens::onUpdate(float dt) {
  auto cnt= CC_GDV(c::Float, state, "secCount");
  auto node = ships->head;
  if (NNP(node)) {
    doIt(node, cnt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Aliens::doIt(a::Node* node, int dt) {
  auto enemies= MGMS()->getPool("Baddies");
  auto cfg= MGMS()->getLCfg()->getValue();

  if (enemies->countActives() <
      cfg["enemyMax"].get<j::json::number_integer_t>()) {

    j::json arr= cfg["enemies"].get<j::json::array_t>();
    J__LOOP(it, arr) {
      auto& a = *it;
      auto time = a["time"].get<j::json::number_integer_t>();
      auto style = a["style"].get<j::json::string_t>();
        j::json types =  a["types"].get<j::json::array_t>();
      auto fc = [=]() {
        J__LOOP(t, types) {
          auto& v = *t;
          this->addEnemyToGame(node, v.get<j::json::number_integer_t>());
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
    return (e->enemyType.type == arg.type &&
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
    y->sprite->schedule([=](float) {
      this->dropBombs(y);
    }, y->delayTime, "dropBombs");
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
  auto sz= en->csize();
  auto epos= en->pos();
  c::Action* act;

  en->setPos(cx::randFloat(wz.size.width *0.5f + 80.0f), wz.size.height);
  switch (en->moveType) {

      case Moves::RUSH: {
          act = c::MoveTo::create(1, c::ccp(pos.x, pos.y));
      }
    break;

      case Moves::VERT: {
          act = c::MoveBy::create(4, c::ccp(0, -wz.size.height - sz.height)); }
    break;

      case Moves::HORZ: {
          auto a0 = c::MoveBy::create(0.5f, c::ccp(0, -100 - cx::randInt(200)));
          auto a1 = c::MoveBy::create(1, c::ccp(-50 - cx::randInt(100), 0));
      auto cb = [=]() {
        auto a2 = c::DelayTime::create(1);
          auto a3 = c::MoveBy::create(1, c::ccp(100 + cx::randInt(100), 0));
        sprite->runAction(c::RepeatForever::create(
              c::Sequence::create(a2, a3,
                                a2->clone(),
                                a3->reverse())));
      };
          act = c::Sequence::create(a0, a1, c::CallFunc::create(cb));}
    break;

      case Moves::OLAP: {
      auto newX = (pos.x <= wz.size.width * 0.5f) ? wz.size.width : -wz.size.width;
          auto a4 = c::MoveBy::create(4, c::ccp(newX, -wz.size.width * 0.75f));
          auto a5 = c::MoveBy::create(4, c::ccp(-newX, -wz.size.width));
          act = c::Sequence::create(a4,a5);}
    break;


  }

  sprite->runAction(act);
}


NS_END(terra)


