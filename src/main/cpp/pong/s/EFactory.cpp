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
#include "EFactory.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
a::Entity* createBall(float x, float y) {
  auto cfg = MGMS()->getLCfg()->getValue();
  auto sd= JS_FLOAT(cfg["ball+speed"]);
  auto ent = engine->reifyEntity("*");
  auto vy = sd * cx::randSign();
  auto vx = sd * cx::randSign();

  if (MGMS()->isOnline()) {
    vx = 0;
    vy = 0;
  }

  auto sp= cx::reifySprite("pongball.png");
  sp->setPosition(x,y);
  MGML()->addAtlasItem("game-pics", sp);

  ent->checkin(mc_new_2(Velocity, vx, vy));
  ent->checkin(mc_new_1(Ball, sp, sd));

  return ent;
}

//////////////////////////////////////////////////////////////////////////////
//
a::Entity* EFactory::createOnePaddle(
    Player* p, int pnum, float x, float y) {

  auto cfg = MGMS()->getLCfg()->getValue();
  auto sd= JS_FLOAT(cfg["paddle+speed"]);
  auto ent = engine->reifyEntity("*");
  float lp;
  sstr res;

  if (p->color == CC_CSS("P2_COLOR")) {
    res= "green_paddle.png";
  } else {
    res= "red_paddle.png";
  }

  auto sp = cx::reifySprite(res);
  sp->setPosition(x, y);
  MGML()->addAtlasItem("game-pics", sp);

  ent->checkin(mc_new_2(Paddle, sp, p->color, sd));
  ent->checkin(p);

  if (cx::isPortrait()) { lp = x; } else { lp= y; }
  ent->checkin(mc_new_1(Position, lp));

  if (MGMS()->isOnline() &&
      pnum != p->pnum) {
    ent->checkin(mc_new(Faux));
    //only simulate move
  }
  else
  if (p->category == CC_CSV(c::Integer, "BOT")) {
    ent->checkin(mc_new(Faux));
  } else {
    ent->checkin(mc_new(Motion));
  }

  return ent;
}



NS_END(pong)


