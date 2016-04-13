// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

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
  _rocket= _engine->getNodes("f/CGesture")[0];
  _drawing= _engine->getNodes("n/RPath")[0];
  _shared= _engine->getNodes("n/GVars")[0];
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
  auto rmv= CC_GEC(RocketMotion,_rocket,"f/CMove");
  auto rock= CC_GEC(Rocket,_rocket,"f/CPixie");
  auto dw=CC_GEC(RPath,_drawing,"n/RPath");
  auto ss=CC_GEC(GVars,_shared,"n/GVars");
  auto po=MGMS()->getPool("Planets");
  auto ps= po->ls();
  auto wb=cx::visBox();

  //collision with planets
  F__LOOP(it,ps) {
    auto &e= *it;
    auto p= CC_GEC(Planet,e,"f/CPixie");
    auto pos=p->pos();
    if (pow(pos.x - rock->getPositionX(), 2) +
        pow(pos.y - rock->getPositionY(), 2) <=
        pow(rock->radius() * 0.8 + p->radius() * 0.65, 2)) {
      if (rock->isVisible()) {
        SENDMSG("/game/player/killed");
      }
      break;
    }
  }

  //collision with comet
  if (ss->comet->isVisible()) {
    if (pow(ss->comet->getPositionX() - rock->getPositionX(), 2) +
        pow(ss->comet->getPositionY() - rock->getPositionY(), 2) <= pow (rock->radius() , 2)) {
      if (rock->isVisible()) {
        SENDMSG("/game/player/killed");
      }
    }
    ss->comet->setPositionX(ss->comet->getPositionX() + 50 * dt);
    if (ss->comet->getPositionX() > wb.right * 1.5) {
      ss->comet->stopSystem();
      CC_HIDE(ss->comet);
    }
  }

  //collision with star
  if (pow(ss->star->getPositionX() - rock->getPositionX(), 2) +
      pow(ss->star->getPositionY() - rock->getPositionY(), 2) <=
      pow(rock->radius() * 1.2, 2)) {

    CC_POS1(ss->pickup, ss->star->getPosition());
    ss->pickup->resetSystem();
    if (dw->energy + 0.25 < 1) {
      dw->energy= dw->energy + 0.25;
    } else {
      dw->energy= 1.0;
    }
    dw->setEnergyDecrement(0.0002);
    rmv->speed.x = rmv->speed.x + 2;
    rmv->speed.y= rmv->speed.y;
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
  if (dw->energy == 0) {
    if (rock->isVisible()) {
      SENDMSG("/game/player/killed");
    }
  }

}


NS_END




