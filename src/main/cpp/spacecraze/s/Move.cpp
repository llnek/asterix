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

#pragma once

//////////////////////////////////////////////////////////////////////////////
#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "GEngine.h"
#include "Move.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(spacecraze)

static int POO = 0;

//////////////////////////////////////////////////////////////////////////////
//
void Move::preamble() {
  aliens = engine->getNodeList(AlienNode().typeId());
  ships = engine->getNodeList(ShipNode().typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::processAliens(float dt) {

  auto squad = CC_GNLF(AlienSquad,aliens,"squad");
  auto enemy = squad->aliens->randGet();
    auto wb = cx::visBox();
  auto pos = c::ccpSub(enemy->pos(),
      c::ccp(0, enemy->csize().height * 0.3f));

  auto bullet = cx::reifySprite("sfbullet2");
  MGML()->addAtlasItem("game-pics",bullet);
  //enemy_bullets_->addObject(bullet);

  bullet->setPosition(pos);
  bullet->setScale(0.5f);

  auto scale_up = c::ScaleTo::create(0.25f, 1.0f);
  bullet->runAction(scale_up);

  auto move_down = c::MoveTo::create(
      //enemy->getBulletDuration()
                          1.5f * 0.8f           * (pos.y / wb.top),
      c::ccp(pos.x, 0));

  auto remove = c::CallFunc::create([=]() {

      });

  bullet->runAction(c::Sequence::createWithTwoActions(move_down, remove));

  cx::sfxPlay("shoot_enemy");
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::processShip(float dt) {
  auto ship = CC_GNLF(Ship,ships,"ship");
  auto bx= MGMS()->getEnclosureBox();
  auto pos = ship->pos();
  auto dirty=false;
  auto x= pos.x;
  auto y= pos.y;

  if (MGML()->keyPoll(KEYCODE::KEY_RIGHT_ARROW)) {
    x += ship->vel.x * dt;
    dirty=true;
  }

  if (MGML()->keyPoll(KEYCODE::KEY_LEFT_ARROW)) {
    x -= ship->vel.x * dt;
    dirty=true;
  }

  if (dirty) {
    ship->setPos(cx::clamp(
          c::Vec2(x,y), ship->csize(), bx).x, pos.y);
  }

  if (POO == 0) {
     firePlayerBullet(dt);
     ++POO;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::firePlayerBullet(float dt) {

  auto ship = CC_GNLF(Ship,ships,"ship");
  auto wb= cx::visBox();
  auto pos = c::ccpAdd(ship->pos(),
      c::ccp(0, ship->csize().height * 0.3f));

  auto bullet = cx::reifySprite("sfbullet");
  MGML()->addAtlasItem("game-pics", bullet);

  bullet->setPosition(pos);
  bullet->setScale(0.5f);

  auto scale_up = c::CCScaleTo::create(0.25f, 1.0f);
  bullet->runAction(scale_up);

  // move the bullet up
  auto move_up = c::MoveTo::create(1.5f, c::ccp(pos.x, wb.top));
  auto remove = c::CallFunc::create([=]() {

  });

  bullet->runAction(c::Sequence::createWithTwoActions(move_up, remove));

  cx::sfxPlay("shoot_player");
}

//////////////////////////////////////////////////////////////////////////////
//
bool Move::update(float dt) {
  if (MGMS()->isLive()) {
    //processAliens(dt);
    processShip(dt);
  }
  return true;
}



NS_END





