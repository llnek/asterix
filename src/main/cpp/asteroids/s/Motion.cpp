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

#include "core/XConfig.h"
#include "core/CCSX.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(asteroids)

//////////////////////////////////////////////////////////////////////////////
//
void Motions::preamble() {
  cannonNode = engine->getNodeList(CannonCtrlNode().typeId());
  shipNode= engine->getNodeList(ShipMotionNode().typeId());
  auto mo= CC_GNLF(Motion,shipNode,"motion");
  auto w= CC_CSV(c::Integer, "THROTTLE+WAIT");
  initKeyOps(mo, w);
}

//////////////////////////////////////////////////////////////////////////////
//
bool Motions::update(float dt) {
  if (MGMS()->isLive()) {
    scanInput(dt);
    controlCannon(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Motions::controlCannon(float dt) {
  auto gun = CC_GNLF(Cannon,shipNode,"cannon");
  auto lpr= CC_GNLF(Looper,shipNode, "looper");
  auto ship= CC_GNLF(Ship,shipNode, "ship");

  if (! gun->hasAmmo) {
    if (cx::timerDone(lpr->timer0)) {
      SNPTR( lpr->timer0)
      gun->hasAmmo=true;
    }
  }
  else
  if (MGMS()->keyPoll(KEYCODE::KEY_SPACE)) {
    fireMissile(dt);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Motions::fireMissile(float dt) {
  auto lpr= CC_GNLF(Looper,shipNode,"looper");
  auto gun= CC_GNLF(Cannon,shipNode,"cannon");
  auto ship= CC_GNLF(Ship,shipNode, "ship");
  auto p= MGMS()->getPool("Missiles");
  auto sz= ship->sprite->getContentSize();
  auto deg= ship->sprite->getRotation();
  auto top= cx::getTop(ship->sprite);
  auto pos= ship->pos();
  auto ent= p->get();

  if (ENP(ent)) {
    engine->createMissiles(30);
    ent= p->get();
  }

  auto rc= cx::calcXY(deg, sz.height * 0.5f);
  auto cfg= MGMS()->getLCfg()->getValue();
  ent->vel.x = rc.x;
  ent->vel.y = rc.y;
  ent->inflate( pos.x + rc.x, pos.y + rc.y);
  ent->sprite->setRotation(deg);

  lpr->timer0 = cx::reifyTimer(MGML(), JS_FLOAT(cfg["coolDownWnd"]));
  gun->hasAmmo=false;
}

//////////////////////////////////////////////////////////////////////////////
//
void Motions::scanInput(float dt) {
  if (MGMS()->keyPoll(KEYCODE::KEY_RIGHT_ARROW)) {
    rotRight();
  }
  if (MGMS()->keyPoll(KEYCODE::KEY_LEFT_ARROW)) {
    rotLeft();
  }
  if (MGMS()->keyPoll(KEYCODE::KEY_DOWN_ARROW)) {
    sftDown();
  }
  if (MGMS()->keyPoll(KEYCODE::KEY_UP_ARROW)) {
    sftUp();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Motions::initKeyOps(Motion *mo, int w) {

  rotRight = cx::throttle([=]() {
      mo->right=true;
      }, w);

  rotLeft = cx::throttle([=]() {
      mo->left=true;
      }, w);

  sftDown= cx::throttle([=]() {
      mo->down=true;
      }, w);

  sftUp= cx::throttle([=]() {
      mo->up=true;
      }, w);
}


NS_END(asteroids)


