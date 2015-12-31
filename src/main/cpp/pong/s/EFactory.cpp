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
#include "s/Resolve.h"
#include "s/Collide.h"
#include "s/Motion.h"
#include "s/Move.h"
#include "s/Net.h"
#include "s/Stage.h"
#include "EFactory.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::ignite() {
    auto e = this;
  e->forceSync();

  e->regoSystem(mc_new(Resolve));
  e->regoSystem(mc_new(Collide));
  e->regoSystem(mc_new(Move));
  e->regoSystem(mc_new(Motions));
  e->regoSystem(mc_new(Net));
  e->regoSystem(mc_new(Stage));
}

//////////////////////////////////////////////////////////////////////////////
//
a::Entity* GEngine::mkArena(int cur) {
  auto ent= this->reifyEntity("*");
  auto ss= mc_new(Slots);

  ent->checkin(mc_new(Players));
  ent->checkin(ss);

  ss->poked=false;
  ss->pnum=cur;

  return ent;
}

//////////////////////////////////////////////////////////////////////////////
//
a::Entity* GEngine::mkBall(float x, float y) {
  auto cfg = MGMS()->getLCfg()->getValue();
  auto sd= JS_FLOAT(cfg["ball+speed"]);
  auto ent = this->reifyEntity("*");
  auto vy = sd * cx::randSign();
  auto vx = sd * cx::randSign();

  if (MGMS()->isOnline()) {
    vx = 0;
    vy = 0;
  }

  auto sp= cx::reifySprite("pongball.png");
  auto b= mc_new_2(Ball, sp, sd);

  MGML()->addAtlasItem("game-pics", sp);
  sp->setPosition(x,y);
  b->vel.x=vx;
  b->vel.y=vy;
  ent->checkin(b);

  return ent;
}

//////////////////////////////////////////////////////////////////////////////
//
a::Entity* GEngine::mkOnePaddle(int cur, const Player &p, float x, float y) {

  auto cfg = MGMS()->getLCfg()->getValue();
  auto sd= JS_FLOAT(cfg["paddle+speed"]);
  auto ent = this->reifyEntity("*");
  float lp;
  sstr res;

  if (p.color == CC_CSS("P2_COLOR")) {
    res= "green_paddle.png";
  } else {
    res= "red_paddle.png";
  }

  auto sp = cx::reifySprite(res);
  sp->setPosition(x, y);

  ent->checkin(mc_new_3(Paddle, sp, p.pnum, sd));
  ent->checkin(mc_new_1(Player, p));
  MGML()->addAtlasItem("game-pics", sp);

  if (cx::isPortrait()) { lp = x; } else { lp= y; }
  ent->checkin(mc_new_1(Position, lp));

  if (MGMS()->isOnline() &&
      cur != p.pnum) {
    ent->checkin(mc_new(Faux));
    //only simulate move
  }
  else
  if (p.category == CC_CSV(c::Integer, "BOT")) {
    ent->checkin(mc_new(Faux));
  } else {
    //ent->checkin(mc_new(Motion));
  }

  return ent;
}



NS_END(pong)


