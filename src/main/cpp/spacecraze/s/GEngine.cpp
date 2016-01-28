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
#include "Aliens.h"
#include "Move.h"
#include "GEngine.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(spacecraze)


//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {
  MGMS()->reifyPool("Aliens");
  reifyAliens();
  reifyShip();
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initSystems() {
  regoSystem(mc_new1(Aliens,this));
  regoSystem(mc_new1(Move,this));
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::reifyShip() {

  auto p = cx::reifySprite("sfgun");
  auto c= mc_new1(Ship,p);
  auto ent= this->reifyEntity();
  auto z= p->getContentSize();

  ent->checkin(mc_new(Gesture));
  ent->checkin(c);

  MGML()->addAtlasItem("game-pics", p);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::spawnPlayer(Ship* ship) {

  auto wb = cx::visBox();

  ship->inflate(wb.cx, wb.top * -0.1f);

  auto movement = c::EaseBackOut::create(
      c::MoveTo::create(1.0f,
        c::ccp(wb.cx, wb.top * 0.1f)));
  auto movement_over = c::CallFunc::create([=]() {

  });

  ship->sprite->runAction(c::Sequence::createWithTwoActions(
        movement, movement_over));
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::reifyAliens() {
  //char level_file[64] = {0}; //::sprintf(level_file, "Level%02d.xml", MGMS()->getLevel());
  NS_USING(cocos2d)
  auto cl= MGMS()->getLevel();
  auto wz= cx::visRect();
  auto wb= cx::visBox();
  auto mez = cx::calcSize("sfenmy3");
  auto top = wb.top * 0.8f;
  sstr fp = cl < 10 ? "0" : "";
  auto map= cx::readXmlAsDict("pics/Level" + fp + s::to_string(cl) + ".xml");
  auto d= f::dictVal<c::Dictionary>(map, "player");
  auto r1= f::dictVal<c::String>(d, "fireRate")->floatValue();
  auto pool= MGMS()->getPool("Aliens");
  d= f::dictVal<c::Dictionary>(map, "enemy");
  auto dur= f::dictVal<c::String>(d, "moveDuration")->floatValue();
  auto r2= f::dictVal<c::String>(d, "fireRate")->floatValue();
  auto scores= f::dictVal<c::Dictionary>(d, "scores");
  auto layout= f::dictVal<c::Array>(d, "layout");
  c::Ref *ref= nullptr;
  float maxRight=wb.left;
  float minLeft=wb.right;
  CCARRAY_FOREACH(layout, ref) {
    auto s = (c::String*) ref;
    auto v= f::tokenize(s->getCString(), ',');
    float height_t=0;
    float width_t=0;
    float gap=0;
    gap= s::stof(v[0]);
    v.erase(v.begin());
    s_vec<c::Sprite*> aliens;
    auto z= v.size();
    for (auto j=0; j < z; ++j) {
      auto png="sfenmy" + v[j];
      auto sp= cx::reifySprite(png);
      auto sz= sp->getContentSize();
      width_t += sz.width;
      height_t= MAX(sz.height, height_t);
      aliens.push_back(sp);
    }
    width_t += gap * (z-1);

    auto lf= wb.left + 0.5f * (wb.right - wb.left - width_t);
    auto rt= lf + width_t;
    maxRight=MAX(maxRight, rt);
    minLeft=MIN(minLeft,lf);
    F__LOOP(it,aliens) {
      auto sp= *it;
      auto sw= sp->getContentSize().width;
      sp->setPosition(lf + sw * 0.5f, top);
      MGML()->addAtlasItem("game-pics",sp);
      lf += sw + gap;
      pool->checkin(mc_new1(Alien,sp), true);
    }
    top -= height_t * 1.5f;
  }

  auto q= mc_new1(AlienSquad,pool);
  auto ent= this->reifyEntity();
  q->rightEdge= maxRight;
  q->leftEdge= minLeft;
  q->duration=dur;
  q->fireRate=r2;
  ent->checkin(q);
}


NS_END




