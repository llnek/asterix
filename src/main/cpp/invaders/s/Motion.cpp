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
#include "Motion.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////
//
void Motions::preamble() {
  _aliens = _engine->getNodes("n/AlienMotion")[0];
  _cannon = _engine->getNodes("n/CannonCtrl")[0];
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

  auto gun = CC_GEC(Cannon, _cannon, "n/Cannon");
  auto lpr= CC_GEC(Looper, _cannon, "n/Looper");
  auto ship= CC_GEC(f::CDraw, _cannon, "f/CDraw");
  auto t= lpr->timer7;

  //throttle the cannon with timer
  if (!cx::timerDone(t)) { return;}
  if (gun->hasAmmo) { return;}

  ship->node->setSpriteFrame(node->frame0);
  gun->hasAmmo=true;
  cx::undoTimer(t);
  SNPTR(lpr->timer7)

  auto cfg= MGMS()->getLCfg()->getValue();
  auto p= MGMS()->getPool("Missiles");
  auto top= cx::getTop(ship->node);
  auto pos= ship->pos();
  auto ent= p->get();

  if (ENP(ent)) {
    SCAST(GEngine*,engine)->reifyMissiles(36);
    ent= p->get();
  }

  ent->inflate(pos.x, top+4);

  lpr->timer7 = cx::reifyTimer( MGML(), JS_FLOAT(cfg["coolDownWindow"]));
  gun->hasAmmo=false;
  ship->node->setSpriteFrame(ship->frame1);

  cx::sfxPlay("ship-missile");
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::processAlienMotions(float) {

  auto sqad= CC_GEC(AlienSquad, _aliens, "n/AlienSquad");
  auto lpr = CC_GEC(Looper, _aliens, "n/Looper");
  auto cfg= MGMS()->getLCfg()->getValue();

  if (ENP(lpr->timer0)) {
    lpr->timer0= cx::reifyTimer(MGML(), JS_FLOAT(cfg["marching"]));
  }

  if (ENP(lpr->timer1)) {
    lpr->timer1= cx::reifyTimer(MGML(), JS_FLOAT(cfg["bombing"]));
  }
}


NS_END


