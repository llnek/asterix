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
#include "Move.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(prototype)


//////////////////////////////////////////////////////////////////////////////
//
void Move::preamble() {
  _player= _engine->getNodes("f/CGesture")[0];
  _shared= _engine->getNodes("n/GVars")[0];
}

//////////////////////////////////////////////////////////////////////////////
//
bool Move::update(float dt) {
  if (MGMS()->isLive()) {
    process(dt);
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::process(float dt) {
  auto ss= CC_GEC(GVars, _shared, "n/GVars");
  auto po= MGMS()->getPool("Asteroids");
  auto c=po->ls();
  auto wz= cx::visRect();
  auto wb= cx::visBox();

  F__LOOP(it,c) {
    auto a= *it;
    if (!a->status()) { continue; }
    auto r= CC_GEC(f::CPixie,a,"f/CPixie");

    r->setPos(
        r->node->getPositionX(),
        r->node->getPositionY() - (0.75 * CC_ZH(wz.size) * dt));

    if (r->node->getPositionY() < wb.bottom) {
      if (r->node->getPositionY() <
          wb.bottom-(2* CC_ZH(r->csize()))) {
        r->node->getPhysicsBody()->setEnabled(false);
        cx::hibernate((ecs::Node*)a);
      }
    }
  }

  onKeys(dt);
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::onKeys(float dt) {

  auto r= CC_GEC(f::CPixie, _player, "f/CPixie");
  auto m= CC_GEC(f::CMove, _player, "f/CMove");
  auto pos= r->pos();
  auto x=pos.x;
  auto y=pos.y;

  if (MGML()->keyPoll(KEYCODE::KEY_RIGHT_ARROW)) {
    x = pos.x + dt * m->speed.x;
  }
  if (MGML()->keyPoll(KEYCODE::KEY_LEFT_ARROW)) {
    x = pos.x - dt * m->speed.x;
  }
  if (MGML()->keyPoll(KEYCODE::KEY_UP_ARROW)) {
    y = pos.y + dt * m->speed.y;
  }
  if (MGML()->keyPoll(KEYCODE::KEY_DOWN_ARROW)) {
    y = pos.y - dt * m->speed.y;
  }

  r->setPos(x,y);
}


NS_END




