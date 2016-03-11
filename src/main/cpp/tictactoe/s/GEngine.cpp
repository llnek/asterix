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
#include "Resolve.h"
#include "Net.h"
#include "Logic.h"
#include "n/lib.h"
#include "GEngine.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initSystems() {
  regoSystem(mc_new1(Resolve,this));
  regoSystem(mc_new1(Logic,this));
  if (MGMS()->isOnline()) {
    regoSystem(mc_new1(Net,this));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {
  auto human = CC_CSV(c::Integer, "HUMAN");
  auto bot = CC_CSV(c::Integer, "BOT");
  auto netp = CC_CSV(c::Integer, "NETP");
  auto xv= CC_CSV(c::Integer, "CV_X");
  auto ov= CC_CSV(c::Integer, "CV_O");
  auto mode = MGMS()->getMode();
  auto css= mc_new(CSquares);
  auto ps= mc_new(Players);
  auto gvs= mc_new(GVars);
  auto gps= mapGridPos(1);
  ecs::Node *arena;
  ecs::Node *board;
  ecs::Node *p2;
  ecs::Node *p1;
  auto cat1= human;
  auto cat2= bot;
  ArrCells seed;
  seed.fill(0);

  arena= this->reifyNode();
  board= this->reifyNode();
  p2 = this->reifyNode();
  p1 = this->reifyNode();

  // config global vars
  gvs->pnum= mynum;

  // the squares
  for (auto i=0; i < css->sqs.size(); ++i) {
    css->sqs[i] = mc_new1(CSquare, i);
  }
  S__COPY(gps, css->boxes);

  arena->checkin(gvs);
  arena->checkin(css);

  // config p2 and p1
  if (mode == f::GMode::NET) {
    cat2= netp;
    cat1= netp;
    p2->checkin(mc_new(f::CHuman));
    p1->checkin(mc_new(f::CHuman));
  }
  else
  if (mode == f::GMode::ONE) {
    p2->checkin( mc_new2(TTToe, xv, ov));
    p2->checkin(mc_new(f::CAutoma));
    p1->checkin(mc_new(f::CHuman));
  }
  else
  if (mode == f::GMode::TWO) {
    cat2= human;
    cat1= human;
    p2->checkin(mc_new(f::CHuman));
    p1->checkin(mc_new(f::CHuman));
  }

  ps->parr[2]= mc_new3(Player, cat2, ov, 2);
  ps->parr[2]->color= CC_CSS("P2_COLOR");
  ps->parr[1]= mc_new3(Player, cat1, xv, 1);
  ps->parr[1]->color= CC_CSS("P1_COLOR");
  ps->parr[0]= mc_new1(Player,mynum);

  p2->checkin(ps->parr[2]);
  p1->checkin(ps->parr[1]);

  // the board
  board->checkin(mc_new1( Grid, seed));
  board->checkin(ps);
  board->checkin(mc_new(CellPos));
}




NS_END


