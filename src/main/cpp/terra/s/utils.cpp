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

#include "CObjs.h"
#include "utils.h"
NS_BEGIN(terra)


//////////////////////////////////////////////////////////////////////////////
//
void flareEffect(c::Sprite* flare, VOIDFN cb) {

  auto flareY = CC_CSV(c::Integer, "flareY");

  flare->setBlendFunc(c::BlendFunc::ADDICTIVE);
  flare->stopAllActions();
  flare->setPosition(-45, flareY);
  flare->setVisible(true);
  flare->setOpacity(0);
  flare->setRotation(-120);
  flare->setScale(0.3f);

  auto opacityAnim = c::FadeTo::create(0.5f, 255);
  auto opacDim = c::FadeTo::create(1, 0);

  auto rotateEase = c::EaseExponentialOut::create( c::RotateBy::create(2.5f, 90));
  auto easeMove = c::EaseSineOut::create(c::MoveBy::create(0.5f, cc.p(490, 0));
  auto biggerEase = c::EaseSineOut::create(c::ScaleBy(0.7f, 1.2f, 1.2f));
  auto bigger = c::ScaleTo::create(0.5f, 1);
  auto onComplete = c::CallFunc::create(cb);
  auto killflare = c::CallFunc::create([=]() {
    flare->removeFromParent();
  });

  flare->runAction(c::Sequence::create(opacityAnim,
      biggerEase, opacDim,
      killflare, onComplete));

  flare->runAction(easeMove);
  flare->runAction(rotateEase);
  flare->runAction(bigger);
}

//////////////////////////////////////////////////////////////////////////////
//
void btnEffect() {
  cx::sfxPlay("btnEffect");
}

//////////////////////////////////////////////////////////////////////////////
//
void fireMissiles(f::ComObj* obj, float dt) {
  auto po1= MGMS()->getPool("missiles");
  auto ship = (Ship*) obj;
  auto pos = ship->pos();
  auto sz = ship->size();
  auto offy= 3.0f + sz.height * 0.3f;
  auto offx=13.0f;
  auto m2= po1->getAndSet();
  auto m1= po1->getAndSet();

  if (!m1 || !m2) { EFactory::createMissiles(); }

  if (!m1) { m1= po1->getAndSet(); }
  if (!m2) { m2= po1->getAndSet(); }

  m2->inflate( pos.x - offx, pos.y + offy );
  m1->inflate( pos.x + offx, pos.y + offy );
}

//////////////////////////////////////////////////////////////////////////////
//
void bornShip(Ship* ship) {
  auto bsp= ship->bornSprite;
  auto ssp= ship->sprite;
  auto normal = c::CallFunc::create([=]() {
    ship->canBeAttack = true;
    bsp->setVisible(false);
    ssp->schedule([=](float dt) {
      fireMissiles(ship, dt);
    }, 1/6);
    ship->inflate();
  });

  ship->canBeAttack = false;
  bsp->scale = 8.0f;
  bsp->setVisible(true);
  bsp->runAction(c::ScaleTo::create(0.5f, 1, 1));

  ssp->runAction(c::Sequence::create(c::DelayTime::create(0.5f),
        c::Blink(3,9), normal));
}

//////////////////////////////////////////////////////////////////////////////
//
void processTouch(f::ComObj* ship, const c::Vec2& delta) {
  auto pos = ship->pos();
  auto wz= cx::visRect();
  auto cur= ccpAdd(pos, delta);
  cur= ccpClamp(cur, ccp(0.0f, 0.0f),
                 ccp(wz.width, wz.height));
  ship->setPos(cur.x, cur.y);
}




NS_END(terra)

