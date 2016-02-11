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

//////////////////////////////////////////////////////////////////////////////

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Collide.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(rocket)


//////////////////////////////////////////////////////////////////////////////
//
void Collide::preamble() {
  drawings=engine->getNodeList(LineDrawingNode().typeId());
  shared=engine->getNodeList(SharedNode().typeId());
  rockets=engine->getNodeList(RocketNode().typeId());

}

//////////////////////////////////////////////////////////////////////////////
//
bool Collide::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Collide::process(float dt) {
  auto dw=CC_GNLF(LineDrawing,drawings,"drawing");
  auto rock= CC_GNLF(Rocket,rockets,"rocket");
  auto ss=CC_GNLF(GVars,shared,"slots");
  auto pool=MGMS()->getPool("Planets");
  auto &ps= pool->list();
  auto wb=cx::visBox();

  //update jet particle so it follow rocket
  //track collision with sides
  rock->collidedWithSides();
  rock->update(dt);
  if (!ss->jet->isActive()) {
    ss->jet->resetSystem();
  }
  ss->jet->setRotation(rock->sprite->getRotation());
  ss->jet->setPosition(rock->pos());

  dw->lines->update(dt);
  rock->sprite->setOpacity(dw->lines->energy * 255);

  //collision with planets
  F__LOOP(it,ps) {
    auto p= *it;
    auto pos=p->pos();
    if (pow(pos.x - rock->sprite->getPositionX(), 2)
        + pow(pos.y - rock->sprite->getPositionY(), 2) <=
        pow(rock->radius() * 0.8f + p->radius() * 0.65f, 2)) {
      if (rock->sprite->isVisible()) {
        SENDMSG("/game/player/killed");
      }
      break;
    }
  }

  //collision with star
  if (pow(ss->star->getPositionX() - rock->sprite->getPositionX(), 2)
      + pow(ss->star->getPositionY() - rock->sprite->getPositionY(), 2) <=
      pow(rock->radius() * 1.2f, 2)) {

    ss->pickup->setPosition(ss->star->getPosition());
    ss->pickup->resetSystem();
    if (dw->lines->energy + 0.25f < 1) {
      dw->lines->energy= dw->lines->energy + 0.25f;
    } else {
      dw->lines->energy= 1.0f;
    }
    dw->lines->setEnergyDecrement(0.0002f);
    rock->speed.x = rock->speed.x + 2;
    rock->speed.y= rock->speed.y;
    cx::sfxPlay("pickup");
    SENDMSG("/game/resetstar");

    int points = 100 - ss->timeBetweenPickups;
    if (points < 0) { points = 0; }
    auto msg= j::json({
        {"score", points}
        });
    SENDMSGEX("/game/player/earnscore", &msg);
    ss->timeBetweenPickups = 0;
  }

  ss->timeBetweenPickups += dt;
  if (dw->lines->energy == 0) {
    if (rock->sprite->isVisible()) {
      SENDMSG("/game/player/killed");
    }
  }

}


NS_END




