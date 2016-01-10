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
#include "Move.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(asteroids)

//////////////////////////////////////////////////////////////////////////////
//
void Move::preamble() {
  shipNode = engine->getNodeList(ShipMotionNode().typeId());
  arenaNode = engine->getNodeList(ArenaNode().typeId());
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::moveXXX(f::XPool *p, float dt) {
  p->foreach([=](f::ComObj *a) {
    if (a->status) { this->moveAstros(a, dt); }
  });
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::moveBBB(f::XPool *p, float dt) {
  p->foreach([=](f::ComObj *m) {
    if (m->status) {
        auto pos= m->pos();
        auto y = pos.y + dt * m->vel.y * m->speed.y;
        auto x = pos.x + dt * m->vel.x * m->speed.x;
        m->setPos(x, y);
      }
  });
}

//////////////////////////////////////////////////////////////////////////////
//
bool Move::update(float dt) {
  if (MGMS()->isLive()) {
    moveXXX(MGMS()->getPool("Astros3"), dt);
    moveXXX(MGMS()->getPool("Astros2"), dt);
    moveXXX(MGMS()->getPool("Astros1"), dt);
    processShipMotions(dt);
    moveBBB(MGMS()->getPool("Missiles"), dt);
    moveBBB(MGMS()->getPool("Lasers"), dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
float Move::rotateShip(float cur, float deg) {
  cur += deg;
  if (cur >= 360) {
    cur = cur - 360;
  }
  if (cur < 0) {
    cur = 360 + cur;
  }
  return cur;
}

//////////////////////////////////////////////////////////////////////////////
//
const c::Vec2 Move::thrust(float angle, float power) {
  return cx::calcXY(angle, power);
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::processShipMotions(float dt) {
  auto ship= CC_GNLF(Ship,shipNode, "ship");
  auto sp = ship->sprite;
  auto pos = sp->getPosition();
  auto x= pos.x;
  auto y= pos.y;

  if (MGML()->keyPoll(KEYCODE::KEY_RIGHT_ARROW)) {
    ship->angle= rotateShip(ship->angle, 3);
    sp->setRotation(ship->angle);
  }

  if (MGML()->keyPoll(KEYCODE::KEY_LEFT_ARROW)) {
    ship->angle= rotateShip(ship->angle, -3);
    sp->setRotation(ship->angle);
  }

  if (MGML()->keyPoll(KEYCODE::KEY_UP_ARROW)) {
    auto acc= thrust(ship->angle, ship->power);
    sp->setSpriteFrame(ship->frame1);
    ship->acc.x= acc.x;
    ship->acc.y= acc.y;
  } else {
    sp->setSpriteFrame(ship->frame0);
  }
  moveShip(dt);
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::moveShip(float dt) {
  auto ship = CC_GNLF(Ship,shipNode,"ship");
  auto B = MGMS()->getEnclosureBox();
  auto sp= ship->sprite;
  auto sz = sp->getContentSize();
  auto pos= sp->getPosition();
    float x,y;
    
  ship->vel.y = ship->vel.y + dt * ship->acc.y;
  ship->vel.x = ship->vel.x + dt * ship->acc.x;

  if (ship->vel.y > ship->maxVel.y) {
    ship->vel.y = ship->maxVel.y;
  }
  else
  if (ship->vel.y < - ship->maxVel.y) {
    ship->vel.y = - ship->maxVel.y;
  }

  if (ship->vel.x > ship->maxVel.x) {
    ship->vel.x = ship->maxVel.x;
  }
  else
  if (ship->vel.x < -ship->maxVel.x) {
    ship->vel.x = -ship->maxVel.x;
  }

  y = pos.y + dt * ship->vel.y;
  x = pos.x + dt * ship->vel.x;

  sp->setPosition(x,y);

  //wrap?
  auto r= cx::bbox4(sp);

  if (r.bottom > B.top) {
    if (ship->vel.y > 0) {
      y = B.bottom - sz.height;
    }
  }

  if (r.top < B.bottom) {
    if (ship->vel.y < 0) {
      y = B.top + sz.height;
    }
  }

  if (r.left > B.right) {
    if (ship->vel.x > 0) {
      x = B.left - sz.width;
    }
  }

  if (r.right < B.left) {
    if (ship->vel.x < 0) {
      x = B.right + sz.width;
    }
  }

  sp->setRotation(ship->angle);
  sp->setPosition(x,y);
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::moveAstros(f::ComObj *co, float dt) {
  auto B = MGMS()->getEnclosureBox();
  auto astro = (f::DynaObj*) co;
  auto rot= astro->angle;
  auto sp= astro->sprite;
  auto sz= sp->getContentSize();
  auto pos= sp->getPosition();
  auto x = pos.x + dt * astro->vel.x;
  auto y = pos.y + dt * astro->vel.y;

  rot += 0.1f;
  if (rot > 360) { rot -= 360; }

  sp->setRotation(rot);
  astro->angle = rot;
  sp->setPosition(x,y);

  //wrap?
  auto r= cx::bbox4(sp);

  if (r.bottom > B.top) {
    if (astro->vel.y > 0) {
      y = B.bottom - sz.height;
    }
  }

  if (r.top < B.bottom) {
    if (astro->vel.y < 0) {
      y = B.top + sz.height;
    }
  }

  if (r.left > B.right) {
    if (astro->vel.x > 0) {
        x = B.left - sz.width;
      }
    }

  if (r.right < B.left) {
    if (astro->vel.x < 0) {
      x = B.right + sz.width;
    }
  }

  sp->setPosition(x,y);
}


NS_END(asteroids)



