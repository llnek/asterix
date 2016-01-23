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
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Motion.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////
//
void Motions::preamble() {
  aliens = engine->getNodeList(AlienMotionNode().typeId());
  cannon = engine->getNodeList(CannonCtrlNode().typeId());
}

//////////////////////////////////////////////////////////////////////////
//
bool Motions::update(float dt) {

  if (MGMS()->isLive()) {
    processAlienMotions(dt);
    processCannon(dt);
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::processCannon(float dt) {

  auto gun = CC_GNLF(Cannon, cannon, "cannon");
  auto lpr= CC_GNLF(Looper, cannon, "looper");
  auto ship= CC_GNLF(Ship, cannon, "ship");
  auto t= lpr->timer7;

  //throttle the cannon with timer
  if (gun->hasAmmo) { return;}
  if (!cx::timerDone(t)) { return;}

  ship->sprite->setSpriteFrame(ship->frame0);
  gun->hasAmmo=true;
  cx::undoTimer(t);
  SNPTR(lpr->timer7)

  auto cfg= MGMS()->getLCfg()->getValue();
  auto p= MGMS()->getPool("missiles");
  auto top= cx::getTop(ship->sprite);
  auto pos= ship->pos();
  auto ent= p->get();

  if (ENP(ent)) {
    SCAST(GEngine*,engine)->reifyMissiles(36);
    ent= p->get();
  }

  ent->inflate(pos.x, top+4);

  lpr->timer7 = cx::reifyTimer( MGML(), JS_FLOAT(cfg["coolDownWindow"]));
  gun->hasAmmo=false;
  ship->sprite->setSpriteFrame(ship->frame1);

  cx::sfxPlay("ship-missile");
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::processAlienMotions(float) {

  auto sqad= CC_GNLF(AlienSquad, aliens, "aliens");
  auto lpr = CC_GNLF(Looper, aliens, "looper");
  auto cfg= MGMS()->getLCfg()->getValue();

  if (ENP(lpr->timer0)) {
    lpr->timer0= cx::reifyTimer(MGML(), JS_FLOAT(cfg["marching"]));
  }

  if (ENP(lpr->timer1)) {
    lpr->timer1= cx::reifyTimer(MGML(), JS_FLOAT(cfg["bombing"]));
  }
}


NS_END(invaders)


