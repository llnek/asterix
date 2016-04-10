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
  _parr[2]= p2;
  _parr[1]= p1;
  _parr[0].pnum=cur;
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
  mkOnePaddle(_parr[2]);
  mkArena();
  mkBall();
  mkOnePaddle(_parr[1]);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::mkArena() {
  auto ent= this->reifyNode("Arena", true);
  auto ps= mc_new(Players);
  auto ss= mc_new(GVars);

  ss->pnum= _parr[0].pnum;
  ss->poked=false;

  ps->parr[2]= _parr[2];
  ps->parr[1]= _parr[1];
  ps->parr[0]= _parr[0];

  ent->checkin(ps);
  ent->checkin(ss);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::mkBall() {
  auto cfg = MGMS()->getLCfg()->getValue();
  auto sd= JS_FLOAT(cfg["BALL+SPEED"]);
  auto ent = this->reifyNode("Ball", true);
  auto vy = sd * cx::randSign();
  auto vx = sd * cx::randSign();

  if (MGMS()->isOnline()) {
    vy = 0;
    vx = 0;
  }

  auto m= mc_new(f::CMove);
  auto b= Ball::create();

  MGML()->addAtlasItem("game-pics", b);
  m->vel.x=vx;
  m->vel.y=vy;

  ent->checkin(m);
  ent->checkin(b);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::mkOnePaddle(const Player &p) {

  auto ent = this->reifyNode("Paddle", true);
  auto cfg = MGMS()->getLCfg()->getValue();
  auto sd= JS_FLOAT(cfg["PADDLE+SPEED"]);
  auto cur= _parr[0].pnum;
  float lp=0;
  sstr res;

  auto sp = Paddle::create(p.pnum);
  auto m= mc_new(f::CMove);

  ent->checkin(mc_new1(Position, lp));
  ent->checkin(mc_new1(Player, p));
  ent->checkin(sp);
  ent->checkin(m);

  MGML()->addAtlasItem("game-pics", sp);
  m->speed.x=sd;

  if (MGMS()->isOnline() && cur != p.pnum) {
    ent->checkin(mc_new(Faux));
    //only simulate move
  }
  else
  if (p.category == CC_CSV(c::Integer, "BOT")) {
    ent->checkin(mc_new(Faux));
  }
  else {
    ent->checkin(mc_new(f::CHuman));
    ent->checkin(mc_new(f::CGesture));
  }

}



NS_END


