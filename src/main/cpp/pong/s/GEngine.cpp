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
#include "s/Move.h"
#include "s/Net.h"
#include "GEngine.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
GEngine::GEngine(int cur, const Player &p1, const Player &p2)  {
  parr[2]= p2;
  parr[1]= p1;
  parr[0].pnum=cur;
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initSystems() {
  regoSystem(mc_new1(Resolve, this));
  regoSystem(mc_new1(Collide, this));
  regoSystem(mc_new1(Move, this));
  regoSystem(mc_new1(Net, this));
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {
  mkOnePaddle(parr[2]);
  mkArena();
  mkBall();
  mkOnePaddle(parr[1]);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::mkArena() {
  auto ent= this->reifyEntity();
  auto ps= mc_new(Players);
  auto ss= mc_new(GVars);

  ss->pnum= parr[0].pnum;
  ss->poked=false;

  ps->parr[2]= parr[2];
  ps->parr[1]= parr[1];
  ps->parr[0]= parr[0];

  ent->checkin(ps);
  ent->checkin(ss);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::mkBall() {
  auto cfg = MGMS()->getLCfg()->getValue();
  auto sd= JS_FLOAT(cfg["BALL+SPEED"]);
  auto ent = this->reifyEntity();
  auto vy = sd * cx::randSign();
  auto vx = sd * cx::randSign();

  if (MGMS()->isOnline()) {
    vy = 0;
    vx = 0;
  }

  auto sp= cx::reifySprite("pongball.png");
  auto b= mc_new2(Ball, sp, sd);

  MGML()->addAtlasItem("game-pics", sp);
  b->vel.x=vx;
  b->vel.y=vy;
  ent->checkin(b);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::mkOnePaddle( const Player &p) {

  auto cfg = MGMS()->getLCfg()->getValue();
  auto sd= JS_FLOAT(cfg["PADDLE+SPEED"]);
  auto ent = this->reifyEntity();
  auto cur= parr[0].pnum;
  float lp;
  sstr res;

  if (p.pnum == 2) {
    res= "green_paddle.png";
  } else {
    res= "red_paddle.png";
  }

  auto sp = cx::reifySprite(res);

  ent->checkin(mc_new3(Paddle, sp, p.pnum, sd));
  ent->checkin(mc_new1(Player, p));
  MGML()->addAtlasItem("game-pics", sp);

  ent->checkin(mc_new1(Position, lp));

  if (MGMS()->isOnline() && cur != p.pnum) {
    ent->checkin(mc_new(Faux));
    //only simulate move
  }
  else
  if (p.category == CC_CSV(c::Integer, "BOT")) {
    ent->checkin(mc_new(Faux));
  }
  else {
    ent->checkin(mc_new(Gesture));
  }

}



NS_END(pong)


