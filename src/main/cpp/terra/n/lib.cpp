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
#include "lib.h"
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
//
void flareEffect(not_null<c::Sprite*> flare, VOIDFN cb) {

  auto opacityAnim = c::FadeTo::create(0.5, 255);
  auto opacDim = c::FadeTo::create(1, 0);
  auto flareY = CC_CSV(c::Integer, "flareY");

  flare->setBlendFunc(BDFUNC::ADDITIVE);
  flare->stopAllActions();
  flare->setPosition(-45, flareY);
  CC_SHOW(flare);
  flare->setOpacity(0);
  flare->setRotation(-120);
  flare->setScale(0.3);

  auto rotateEase = c::EaseExponentialOut::create( c::RotateBy::create(2.5, 90));
  auto easeMove = c::EaseSineOut::create( c::MoveBy::create(0.5, c::ccp(490, 0)));
  auto biggerEase = c::EaseSineOut::create(c::ScaleBy::create(0.7, 1.2, 1.2));
  auto bigger = c::ScaleTo::create(0.5, 1);
  auto kf= [=]() { flare->removeFromParent(); };

  flare->runAction(
      c::Sequence::create(
        opacityAnim, biggerEase, opacDim,
        c::CallFunc::create(kf),
        c::CallFunc::create(cb),
        CC_NIL));

  flare->runAction(easeMove);
  flare->runAction(rotateEase);
  flare->runAction(bigger);
}

//////////////////////////////////////////////////////////////////////////////
//
void fireMissiles(GEngine *eg, not_null<ecs::Node*> obj, float dt) {
  auto ship = CC_GEC(f::CPixie,obj.get(),"f/CPixie");
  auto po1= MGMS()->getPool("Missiles");
  auto sz = ship->csize();
  auto pos = ship->pos();
  auto offy= 3.0 + sz.height * 0.3;
  auto offx= 13.0;
  auto m2= po1->take(true);
  auto m1= po1->take(true);
  auto s2=CC_GEC(f::CPixie,m2,"f/CPixie");
  auto s1=CC_GEC(f::CPixie,m1,"f/CPixie");
  s2->inflate( pos.x - offx, pos.y + offy );
  s1->inflate( pos.x + offx, pos.y + offy );
}

//////////////////////////////////////////////////////////////////////////////
//
void bornShip(GEngine *eg, not_null<ecs::Node*> ccc) {
  auto ship = CC_GEC(Ship,ccc.get(),"f/CPixie");
  auto normal = [=]() {
    CC_HIDE(ship->bornSprite);
    ship->canBeAttack = true;
    ship->node->schedule([=](float dt) {
      fireMissiles(eg, ccc, dt);
    }, 1.0f/6, "fm");
    ship->inflate();
  };

  ship->bornSprite->setScale(8);
  ship->canBeAttack = false;
  CC_SHOW(ship->bornSprite);
  ship->bornSprite->runAction(
      c::ScaleTo::create(0.5, 1, 1));

  ship->node->runAction(
      c::Sequence::create(c::DelayTime::create(0.5),
        c::Blink::create(3,9),
        c::CallFunc::create(normal), CC_NIL));
}

//////////////////////////////////////////////////////////////////////////////
//
void processTouch(not_null<ecs::Node*> ship, const c::Vec2& delta) {
  auto sp=CC_GEC(Ship,ship,"f/CPixie");
  auto bx= MGMS()->getEnclosureBox();
  auto pos= sp->pos();
  auto cur= c::ccpAdd(pos, delta);
  cur= cx::clamp(cur, bx);
  sp->setPos(cur.x, cur.y);
}


NS_END

