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
  _aliens = _engine->getNodes("n/AlienSquad")[0];
  _ship = _engine->getNodes("f/CGesture")[0];
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

  auto lpr= CC_GEC(f::Looper, _ship, "f/Looper");
  auto gun = CC_GEC(ShipStats, _ship, "f/CStats");
  auto ship= CC_GEC(Ship, _ship, "f/CPixie");
  auto &t= lpr->tm;

  //throttle the cannon with timer
  if (cx::timerDone(t.timer)) {
    gun->hasAmmo=true;
    cx::undoTimer(t.timer);
      t.timer=CC_NIL;
  }

  if (!gun->hasAmmo) {
  return;}

  // release a missile
  auto cfg= MGMS()->getLCfg()->getValue();
  auto p= MGMS()->getPool("Missiles");
  auto top= cx::getTop(ship);
  auto pos= ship->pos();
  auto ent= p->take(true);

  t.timer = cx::reifyTimer(MGML(), JS_FLOAT(cfg["coolDownWindow"]));
  gun->hasAmmo=false;
  cx::resurrect((ecs::Node*)ent, pos.x, top+4);

  cx::sfxPlay("ship-missile");
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::processAlienMotions(float) {

  auto sqad= CC_GEC(AlienSquad, _aliens, "n/AlienSquad");
  auto lpr = CC_GEC(f::Loopers, _aliens, "f/Loopers");
  auto cfg= MGMS()->getLCfg()->getValue();

  auto &t1= lpr->tms[0];
  if (E_NIL(t1.timer)) {
    t1.timer= cx::reifyTimer(MGML(), JS_FLOAT(cfg["marching"]));
  }

  auto &t2= lpr->tms[1];
  if (E_NIL(t2.timer)) {
    t2.timer= cx::reifyTimer(MGML(), JS_FLOAT(cfg["bombing"]));
  }
}


NS_END


