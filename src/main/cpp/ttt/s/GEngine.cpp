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

#include "core/XConfig.h"
#include "Resolve.h"
#include "Net.h"
#include "Logic.h"
#include "utils.h"
#include "GEngine.h"
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initSystems() {
  regoSystem(mc_new_1(Resolve,this));
  regoSystem(mc_new_1(Logic,this));
 // regoSystem(mc_new_1(Net,this));
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {
  auto human = CC_CSV(c::Integer, "HUMAN");
  auto bot = CC_CSV(c::Integer, "BOT");
  auto netp = CC_CSV(c::Integer, "NETP");
  auto nil= CC_CSV(c::Integer, "CV_Z");
  auto xv= CC_CSV(c::Integer, "CV_X");
  auto ov= CC_CSV(c::Integer, "CV_O");
  auto mode = MGMS()->getMode();
  auto css= mc_new(CSquares);
  auto ps= mc_new(Players);
  auto gvs= mc_new(GVars);
  a::Entity *arena;
  a::Entity *board;
  a::Entity *p2;
  a::Entity *p1;
  auto cat1= human;
  auto cat2= bot;
  ArrCells seed;
  seed.fill(nil);

  arena= this->reifyEntity();
  board= this->reifyEntity();
  p2 = this->reifyEntity();
  p1 = this->reifyEntity();

  // config global vars
  gvs->pnum= mynum;

  // the squares
  for (auto i=0; i < css->sqs.size(); ++i) {
    css->sqs[i] = mc_new_1(CSquare, i);
  }

  arena->checkin(gvs);
  arena->checkin(css);

  // config p2 and p1
  if (mode == f::GMode::NET) {
    cat2= netp;
    cat1= netp;
    p2->checkin(mc_new(Gesture));
    p1->checkin(mc_new(Gesture));
  }
  else
  if (mode == f::GMode::ONE) {
    auto bd= mc_new_3(TTToe, nil, xv, ov);
    p2->checkin(mc_new_1(SmartAI, bd));
    p1->checkin(mc_new(Gesture));
  }
  else
  if (mode == f::GMode::TWO) {
    cat2= human;
    cat1= human;
    p2->checkin(mc_new(Gesture));
    p1->checkin(mc_new(Gesture));
  }

  ps->parr[2]= mc_new_3(Player, cat2, ov, 2);
  ps->parr[2]->color= CC_CSS("P2_COLOR");
  ps->parr[1]= mc_new_3(Player, cat1, xv, 1);
  ps->parr[1]->color= CC_CSS("P1_COLOR");
  ps->parr[0]= mc_new_1(Player,mynum);

  p2->checkin(ps->parr[2]);
  p1->checkin(ps->parr[1]);

  // the board
  board->checkin(mc_new_1( Grid, seed));
  board->checkin(ps);
  board->checkin(mc_new(CellPos));
};

NS_END(tttoe)


