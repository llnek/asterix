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
NS_BEGIN(asteroids)

//////////////////////////////////////////////////////////////////////////////
//
void Motions::preamble() {
  cannon = engine->getNodeList(CannonCtrlNode().typeId());
  ship= engine->getNodeList(ShipMotionNode().typeId());
  arena= engine->getNodeList(ArenaNode().typeId());

  auto mo= CC_GNLF(Gesture,ship,"motion");
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
  auto gun = CC_GNLF(Cannon,cannon,"cannon");
  auto lpr= CC_GNLF(Looper,cannon, "looper");

  if (! gun->hasAmmo) {
    if (cx::timerDone(lpr->timer0)) {
      SNPTR( lpr->timer0)
      gun->hasAmmo=true;
    }
  }
  else
  if (MGML()->keyPoll(KEYCODE::KEY_SPACE)) {
    fireMissile(dt);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Motions::fireMissile(float dt) {
  auto lpr= CC_GNLF(Looper,cannon,"looper");
  auto gun= CC_GNLF(Cannon,cannon,"cannon");
  auto sp= CC_GNLF(Ship,ship, "ship");
  auto sz= sp->sprite->getContentSize();
  auto deg= sp->sprite->getRotation();
  auto p= MGMS()->getPool("Missiles");
  auto top= cx::getTop(sp->sprite);
  auto pos= sp->pos();
  auto ent= p->get();

  if (ENP(ent)) {
    SCAST(GEngine*,engine)->createMissiles(30);
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
void Motions::scanInput(Gesture *mo) {
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


NS_END(asteroids)


