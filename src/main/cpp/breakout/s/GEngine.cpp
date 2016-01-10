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

#include "GEngine.h"

NS_BEGIN(breakout)

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {
  createBricks();
  createPaddle();
  createBall();
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initSystems() {
  regoSystem(mc_new(Resolve,this));
  regoSystem(mc_new(Collide,this));
  regoSystem(mc_new(Move,this));
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createBricks() {
  auto cfg= MGMS()->getLCfg()->getValue();
  auto tile = CC_CSV(c::Integer, "TILE");
  auto cg= (Config*) XCFG();
  auto wz = cx::visRect();
  auto cw= cx::center();

  auto loff= CC_CSV(c::Integer,"LEFT+OFF");
  auto csz= cx::calcCize("");
  auto rows= JS_INT(cfg["ROWS"]);
  auto cols= JS_INT(cfg["COLS"])
  auto tr= JS_INT(cfg["TOP+ROW"]);
  auto y= wz.size.height - tr*tile;
  auto x=0.0f;
  auto bf= mc_new(BrickFence);

  for (int r=0; r < rows; ++r) {
    x= csz.width * 0.5f + tile + loff;
    for (int c=0; c < cols; ++c) {
      auto cn= cg->getCandy(r);
      auto sp= cx::reifySprite(cn);
      auto v= JS_INT(cfg["CDS"][cn]);

      MGML()->addAtlasItem("game-pics", sp);
      auto b = mc_new_2(Brick,sp,v);
      bf->bricks.push_back(b);
      sp->inflate(x, y);
      x += csz.width + 1;
    }
    y -= csz.height - 2;
  }

  ent= this->reifyEntity("*");
  ent->checkin(bf);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::bornPaddle(Paddle *p, Ball *ball) {
  auto cw= cx::center();

  ball->inflate(cw.x, 250);
  p->inflate();
  ball->vel.y = ball->speed.y * cx::randSign();
  ball->vel.x = ball->speed.x * cx::randSign();
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createPaddle() {
  auto cfg= MGMS()->getLCfg()->getValue();
  auto sp = cx::reifySprite("paddle.png");
  auto vx= JS_INT(cfg["PADDLE+SPEED"]);
  auto cw= cx::center();
  MGML()->addAtlasItem("game-pics",sp);
  auto p= mc_new_1(Paddle, sp);
  p->speed.x= vx;
  p->vel.x= vx;
  p->inflate( cw.x, 56);
  auto ent= this->reifyEntity("*");
  ent->checkin(p);
  ent->checkin(mc_new(Motion));
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::createBall() {
  auto cfg = MGMS()->getLCfg()->getValue();
  auto bsp= JS_INT(cfg["BALL+SPEED"]);
  auto sp = cx::reifySprite("ball.png");
  auto cw= cx::center();
  auto b= mc_new_1(Ball, sp);

  MGML()->addAtlasItem("game-pics", sp);
  b->speed.x= bsp;
  b->speed.y= bsp;
  b->vel.x= bsp * cx::randSign();
  b->vel.y= bsp * cx::randSign();
  b->inflate( cw.x, 250);

  auto ent= this->reifyEntity("*");
  ent->checkin(b);
}


NS_END(breakout)
#endif


