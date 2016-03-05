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
    engine->getEntities(s_vec<ecs::COMType>{
      "f/CmRender","f/CmMove","f/CmHuman"}, players);
  engine->getEntities("n/GVars",shared);
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
  auto ss= CC_GEC(GVars, shared[0], "n/GVars");
  auto p= MGMS()->getPool("Asteroids");
  auto &c=p->list();
  auto wb= cx::visBox();

  F__LOOP(it,c) {
      auto a= (ecs::Entity*) *it;
    if (!a->status) { continue; }
    auto r= CC_GEC(f::CmRender,a,"f/CmRender");
    r->setPos(r->node->getPositionX(), r->node->getPositionY() - (0.75 * wb.top * dt));
    if (r->node->getPositionY() < wb.bottom-(wb.top * 0.2)) {
      r->node->getPhysicsBody()->setEnabled(false);
      r->deflate();
      a->deflate();
    }
  }

  onKeys(dt);
}

//////////////////////////////////////////////////////////////////////////////
//
void Move::onKeys(float dt) {

  auto r= CC_GEC(f::CmRender, players[0], "f/CmRender");
  auto m= CC_GEC(f::CmMove, players[0], "f/CmMove");
  auto pos= r->pos();
  auto x=pos.x;
  auto y=pos.y;

  if (MGML()->keyPoll(KEYCODE::KEY_RIGHT_ARROW)) {
    x = pos.x + dt * m->speed;
  }
  if (MGML()->keyPoll(KEYCODE::KEY_LEFT_ARROW)) {
    x = pos.x - dt * m->speed;
  }
  if (MGML()->keyPoll(KEYCODE::KEY_UP_ARROW)) {
    y = pos.y + dt * m->speed;
  }
  if (MGML()->keyPoll(KEYCODE::KEY_DOWN_ARROW)) {
    y = pos.y - dt * m->speed;
  }

  r->setPos(x,y);
}


NS_END




