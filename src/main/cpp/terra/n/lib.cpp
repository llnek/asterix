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
#include "GEngine.h"
#include "lib.h"
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
//
void flareEffect(not_null<c::Sprite*> flare, VOIDFN cb) {

  auto opacityAnim = c::FadeTo::create(0.5f, 255);
  auto opacDim = c::FadeTo::create(1, 0);
  auto flareY = CC_CSV(c::Integer, "flareY");

  flare->setBlendFunc(BDFUNC::ADDITIVE);
  flare->stopAllActions();
  flare->setPosition(-45, flareY);
  flare->setVisible(true);
  flare->setOpacity(0);
  flare->setRotation(-120);
  flare->setScale(0.3f);

  auto rotateEase = c::EaseExponentialOut::create( c::RotateBy::create(2.5f, 90));
  auto easeMove = c::EaseSineOut::create( c::MoveBy::create(0.5f, c::ccp(490, 0)));
  auto biggerEase = c::EaseSineOut::create(c::ScaleBy::create(0.7f, 1.2f, 1.2f));
  auto bigger = c::ScaleTo::create(0.5f, 1);
  auto kf= [=]() { flare->removeFromParent(); };

  flare->runAction(
      c::Sequence::create(opacityAnim, biggerEase, opacDim,
        c::CallFunc::create(kf), c::CallFunc::create(cb), nullptr));

  flare->runAction(easeMove);
  flare->runAction(rotateEase);
  flare->runAction(bigger);
}

//////////////////////////////////////////////////////////////////////////////
//
void fireMissiles(not_null<f::ComObj*> obj, float dt) {
  auto po1= MGMS()->getPool("Missiles");
  auto ship = SCAST(Ship*, obj.get());
  auto sz = ship->csize();
  auto pos = ship->pos();
  auto offy= 3.0f + sz.height * 0.3f;
  auto offx= 13.0f;
  auto m2= po1->getAndSet();
  auto m1= po1->getAndSet();

  if (!m1 || !m2) { GEngine::createMissiles(); }

  if (!m1) { m1= po1->getAndSet(); }
  if (!m2) { m2= po1->getAndSet(); }

  m2->inflate( pos.x - offx, pos.y + offy );
  m1->inflate( pos.x + offx, pos.y + offy );
}

//////////////////////////////////////////////////////////////////////////////
//
void bornShip(not_null<f::ComObj*> ccc) {
  auto ship = SCAST(Ship*,ccc.get());
  auto bsp= ship->bornSprite;
  auto ssp= ship->sprite;
  auto normal = [=]() {
    ship->canBeAttack = true;
    bsp->setVisible(false);
    ssp->schedule([=](float dt) {
      fireMissiles(ship, dt);
    }, 1.0f/6, "fm");
    ship->inflate();
  };

  ship->canBeAttack = false;
  bsp->setScale(8.0f);
  bsp->setVisible(true);
  bsp->runAction(c::ScaleTo::create(0.5f, 1, 1));

  ssp->runAction(c::Sequence::create(c::DelayTime::create(0.5f),
        c::Blink::create(3,9),
        c::CallFunc::create(normal),nullptr));
}

//////////////////////////////////////////////////////////////////////////////
//
void processTouch(not_null<f::ComObj*> ship, const c::Vec2& delta) {
  auto bx= MGMS()->getEnclosureBox();
  auto pos= ship->pos();
  auto cur= ccpAdd(pos, delta);
  cur= cx::clamp(cur, bx);
  ship->setPos(cur.x, cur.y);
}


NS_END(terra)

