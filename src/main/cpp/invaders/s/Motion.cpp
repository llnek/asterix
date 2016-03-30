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
  _cannon = _engine->getNodes("n/Cannon")[0];
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

  auto lpr= CC_GEC(f::Looper, _cannon, "f/Looper");
  auto gun = CC_GEC(Cannon, _cannon, "n/Cannon");
  auto ship= CC_GEC(Ship, _cannon, "n/Ship");
  auto t= lpr->timer;

  //throttle the cannon with timer
  if (cx::timerDone(t)) {
    gun->hasAmmo=true;
    cx::undoTimer(t);
    S__NIL(lpr->timer)
  }

  if (!gun->hasAmmo) {
  return;}

  // release a missile
  auto cfg= MGMS()->getLCfg()->getValue();
  auto p= MGMS()->getPool("Missiles");
  auto top= cx::getTop(ship->node);
  auto pos= ship->pos();
  auto ent= (ecs::Node*) p->take(true);
  auto h= CC_GEC(f::CHealth, ent, "f/CHealth");
  auto sp= CC_GEC(f::CPixie, ent, "f/CPixie");
  lpr->timer = cx::reifyTimer(MGML(), JS_FLOAT(cfg["coolDownWindow"]));
  sp->inflate(pos.x, top+4);
  h->reset();
  gun->hasAmmo=false;

  cx::sfxPlay("ship-missile");
}

//////////////////////////////////////////////////////////////////////////
//
void Motions::processAlienMotions(float) {

  auto sqad= CC_GEC(AlienSquad, _aliens, "n/AlienSquad");
  auto lpr = CC_GEC(f::Loopers, _aliens, "f/Loopers");
  auto cfg= MGMS()->getLCfg()->getValue();

  if (ENP(lpr->timers[0])) {
    lpr->timers[0]= cx::reifyTimer(MGML(), JS_FLOAT(cfg["marching"]));
  }

  if (ENP(lpr->timers[1])) {
    lpr->timers[1]= cx::reifyTimer(MGML(), JS_FLOAT(cfg["bombing"]));
  }
}


NS_END


