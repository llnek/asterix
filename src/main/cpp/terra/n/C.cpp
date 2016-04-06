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
#include "GEngine.h"
#include "C.h"

NS_BEGIN(terra)
BEGIN_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
static s::arr<EnemyType, 6> EnemyTypes = {
  EnemyType(Attacks::NORMAL, Moves::RUSH, 0, "E0.png", "W2.png", 1, 15),
  EnemyType(Attacks::NORMAL, Moves::RUSH, 1, "E1.png", "W2.png", 2, 40 ),
  EnemyType(Attacks::TSUIHIKIDAN, Moves::HORZ, 2, "E2.png", "W2.png", 4, 60),
  EnemyType(Attacks::NORMAL, Moves::OLAP, 3, "E3.png", "W2.png", 6, 80 ),
  EnemyType(Attacks::TSUIHIKIDAN, Moves::HORZ, 4, "E4.png", "W2.png", 10, 150 ),
  EnemyType(Attacks::NORMAL, Moves::HORZ, 5, "E5.png", "W2.png", 15, 200 )
};

END_NS_UNAMED
//////////////////////////////////////////////////////////////////////////////
//
owner<Missile*> Missile::create(const sstr &png, Attacks m) {
  auto z= mc_new1(Missile,m);
  z->initWithSpriteFrameName(png);
  z->autorelease();
  return z;
}

//////////////////////////////////////////////////////////////////////////////
//
owner<Ship*> Ship::create() {

}

//////////////////////////////////////////////////////////////////////////////
//
owner<Spark*> Spark::create() {

}

//////////////////////////////////////////////////////////////////////////////
//
owner<Explosion*> Explosion::create() {

}

//////////////////////////////////////////////////////////////////////////////
//
owner<HitEffect*> HitEffect::create() {

}

//////////////////////////////////////////////////////////////////////////////
//
void Spark::inflate(float x, float y) {

  auto scaleBy = c::ScaleBy::create(_duration, 3, 3);
  auto right = c::RotateBy::create(_duration, 45);
  auto seq = c::Sequence::createWithTwoActions(
      c::FadeOut::create(_duration),
      c::CallFunc::create([=]() { this->deflate(); }));

  _sprite2->setRotation( cx::randInt(360));
  _sprite2->setOpacity(255);
  CC_POS2(_sprite2, x, y);
  _sprite2->setScale(_scale);
  CC_SHOW(_sprite2);
  _sprite2->runAction(scaleBy->clone());
  _sprite2->runAction(seq->clone());

  this->setOpacity(255);
  this->setScale(scale);
  this->runAction(right);
  this->runAction(scaleBy);
  this->runAction(seq);

  f::CPixie::inflate(x,y);
}

//////////////////////////////////////////////////////////////////////////////
//
void Spark::deflate() {
  _sprite2->stopAllActions();
  CC_HIDE(_sprite2);
  f::CPixie::deflate();
}

//////////////////////////////////////////////////////////////////////////////
//
void Explosion::inflate(float x, float y) {
  auto ani= CC_ACAC()->getAnimation("Explosion");
  node->runAction(c::Sequence::create(
        c::Animate::create(ani),
        c::CallFunc::create([=]() {
          this->deflate();
        }), CC_NIL));
  f::CPixie::inflate(x, y);
}

//////////////////////////////////////////////////////////////////////////////
//
void HitEffect::inflate(float x, float y) {
  node->runAction(c::ScaleBy::create(0.3, 2, 2));
  node->runAction(
      c::Sequence::create(
        c::FadeOut::create(0.3),
        c::CallFunc::create(
          [=]() { this->deflate(); }),
        CC_NIL));
  node->setRotation(cx::randInt(360));
  node->setScale(_scale);
  f::CPixie::inflate(x,y);
}

//////////////////////////////////////////////////////////////////////////////
//
void flareEffect(not_null<c::Sprite*> flare, VOIDFN cb) {

  auto flareY = CC_CSV(c::Integer, "flareY");

  flare->setBlendFunc(BDFUNC::ADDITIVE);
  flare->stopAllActions();
  CC_POS2(flare, -45, flareY);
  CC_SHOW(flare);
  flare->setOpacity(0);
  flare->setRotation(-120);
  flare->setScale(0.3);

  flare->runAction(
      c::Sequence::create(
        c::FadeTo::create(0.5, 255),
        c::EaseSineOut::create(c::ScaleBy::create(0.7, 1.2, 1.2)),
        c::FadeTo::create(1, 0),
        c::CallFunc::create(
          [=]() { flare->removeFromParent(); }),
        c::CallFunc::create(cb),
        CC_NIL));

  flare->runAction(
    c::EaseSineOut::create(
      c::MoveBy::create(0.5, c::ccp(490, 0))));

  flare->runAction(
      c::EaseExponentialOut::create(
        c::RotateBy::create(2.5, 90)));

  flare->runAction( c::ScaleTo::create(0.5, 1));

}

//////////////////////////////////////////////////////////////////////////////
//
void fireMissiles(not_null<ecs::Node*> obj, float dt) {
  auto ship = CC_GEC(Ship, obj.get(),"f/CPixie");
  auto po1= MGMS()->getPool("Missiles");
  auto pos = ship->getPosition();
  auto sz = CC_CSIZE(ship);
  auto offy= 3.0 + sz.height * 0.3;
  auto offx= 13.0;
  auto m2= po1->take(true);
  auto m1= po1->take(true);
  auto s2=CC_GEC(Missile, m2,"f/CPixie");
  auto s1=CC_GEC(Missile, m1,"f/CPixie");
  cx::resurrect((ecs::Node*)m2, pos.x - offx, pos.y + offy );
  cx::resurrect((ecs::Node*)m1, pos.x + offx, pos.y + offy );
}

//////////////////////////////////////////////////////////////////////////////
//
void bornShip(not_null<ecs::Node*> ccc) {
  auto h = CC_GEC(f::CHealth,ccc.get(),"f/CHealth");
  auto ship = CC_GEC(Ship,ccc.get(),"f/CPixie");
  auto normal = [=]() {
    ship->hideGhost();
    h->exitGodMode();
    ship->schedule([=](float dt) {
      fireMissiles(ccc, dt);
    }, 1.0/6, "fm");
    ship->inflate();
  };
  h->enterGodMode();
  ship->showGhost();
  ship->runAction(
      c::Sequence::create(
        c::DelayTime::create(0.5),
        c::Blink::create(3,9),
        c::CallFunc::create(normal), CC_NIL));
  h->reset();
  ccc->take();
}

//////////////////////////////////////////////////////////////////////////////
//
void processTouch(not_null<ecs::Node*> ship, const CCT_PT &pos) {
  auto sp=CC_GEC(Ship,ship,"f/CPixie");
  auto bx= MGMS()->getEnclosureBox();
  auto cur= cx::clamp(pos, bx);
  CC_POS2(sp, cur.x, cur.y);
}


NS_END


