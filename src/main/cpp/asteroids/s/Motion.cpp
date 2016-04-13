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

#include "x2d/GameScene.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "Motion.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(asteroids)

//////////////////////////////////////////////////////////////////////////////
//
void Motions::preamble() {
  _ship= _engine->getNodes("f/CGesture")[0];
  _arena= _engine->getNodes("n/GVars")[0];
  auto mo= CC_GEC(f::CGesture,_ship,"f/CGesture");
  scanner= cx::throttle(
      [=]() { scanInput(mo); },
      CC_CSV(c::Integer, "THROTTLE+WAIT"));
}

//////////////////////////////////////////////////////////////////////////////
//
bool Motions::update(float dt) {
  if (MGMS()->isLive()) {
    scanner();
    controlCannon(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Motions::controlCannon(float dt) {
  auto lpr= CC_GEC(f::Looper, _ship, "f/Looper");
  auto gun = CC_GEC(Cannon, _ship,"n/Cannon");

  if (gun->hasAmmo) {
    fireMissile(dt);
  }
  else
  if (cx::timerDone(lpr->timer)) {
    cx::undoTimer(lpr->timer);
    S__NIL(lpr->timer)
    gun->hasAmmo=true;
  }
  else
  if (false && MGML()->keyPoll(KEYCODE::KEY_SPACE)) {
    fireMissile(dt);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Motions::fireMissile(float dt) {
  auto lpr= CC_GEC(f::Looper, _ship,"f/Looper");
  auto gun= CC_GEC(Cannon, _ship,"n/Cannon");
  auto sp= CC_GEC(Ship, _ship, "f/CPixie");
  auto p= MGMS()->getPool("Missiles");
  auto cfg= MGMS()->getLCfg()->getValue();
  auto sz= sp->csize();
  auto deg= sp->node->getRotation();
  auto top= cx::getTop(sp->node);
  auto pos= sp->pos();
  auto ent= (ecs::Node*)p->take(true);
  auto mv= CC_GEC(f::CMove,ent,"f/CMove");
  auto dw= CC_GEC(f::CPixie,ent,"f/CPixie");
  auto h= CC_GEC(f::CHealth,ent,"f/CHealth");
  auto rc= cx::calcXY(deg, HHZ(sz));
  mv->vel.x = rc.x;
  mv->vel.y = rc.y;
  h->reset();
  dw->inflate(pos.x + rc.x, pos.y + rc.y);
  dw->node->setRotation(deg);

  lpr->timer = cx::reifyTimer(MGML(), JS_FLOAT(cfg["coolDownWnd"]));
  gun->hasAmmo=false;
}

//////////////////////////////////////////////////////////////////////////////
//
void Motions::scanInput(f::CGesture *mo) {
  if (MGML()->keyPoll(KEYCODE::KEY_RIGHT_ARROW)) {
    mo->right=true;
  }
  if (MGML()->keyPoll(KEYCODE::KEY_LEFT_ARROW)) {
    mo->left=true;
  }
  if (MGML()->keyPoll(KEYCODE::KEY_DOWN_ARROW)) {
    mo->down=true;
  }
  if (MGML()->keyPoll(KEYCODE::KEY_UP_ARROW)) {
    mo->up=true;
  }
}


NS_END


