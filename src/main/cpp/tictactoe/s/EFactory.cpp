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
#include "s/Resolve.h"
#include "s/Stage.h"
#include "s/Logic.h"
#include "EFactory.h"
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initSystems() {
  regoSystem(mc_new_1(Resolve,this));
  regoSystem(mc_new_1(Logic,this));
  regoSystem(mc_new_1(Stage,this));
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {
  reifyArena();
  reifyBoard();
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::reifyArena() {
  auto ent= this->reifyEntity("*");
  auto gvs= mc_new(GVars);
  gvs->pnum=this->pnum;
  ent->checkin(gvs);
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::reifyBoard() {

  auto human = CC_CSV(c::Integer, "HUMAN");
  auto bot = CC_CSV(c::Integer, "BOT");
  auto netp = CC_CSV(c::Integer, "NETP");
  auto nil= CC_CSV(c::Integer, "CV_Z");
  auto xv= CC_CSV(c::Integer, "CV_X");
  auto ov= CC_CSV(c::Integer, "CV_O");
  auto p1c= CC_CSS("P1_COLOR");
  auto p2c= CC_CSS("P2_COLOR");

  auto bd= mc_new_3(Board, nil, xv, ov);
  auto ent= this->reifyEntity("*");
  auto mode = MGMS()->getMode();
  auto ps= mc_new( Players);
  auto p1cat= human;
  auto p2cat= bot;
  ArrCells seed;

  if (mode == f::GMode::NET) {
    p2cat = netp;
    p1cat = netp;
  }
  else
  if (mode == f::GMode::ONE) {
  }
  else
  if (mode == f::GMode::TWO) {
    p2cat= human;
    p1cat= human;
  }
  ps->parr[1] = Player(p1cat, xv, 1, p1c);
  ps->parr[2] = Player(p2cat, ov, 2, p2c);
  ps->parr[0] = Player();

  seed.fill(nil);

  ent->checkin(mc_new_1( PlayView , MGML()));
  ent->checkin(mc_new( CellPos));
  ent->checkin(mc_new_1( Robot, bd));
  ent->checkin(ps);
  ent->checkin(mc_new_1( Grid, seed));

};

NS_END(tttoe)


