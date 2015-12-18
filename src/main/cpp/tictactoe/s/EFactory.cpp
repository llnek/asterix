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
#include "EFactory.h"
#include "n/Board.h"
#include "n/CObjs.h"
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////
//
EFactory::EFactory(not_null<a::Engine*> e,
    not_null<c::Dictionary*> options)

  : Factory(e, options) {
}

//////////////////////////////////////////////////////////////////////////////
//
a::Entity* EFactory::reifyBoard() {

  auto nil= CC_CSV(c::Integer, "CV_Z");
  auto xv= CC_CSV(c::Integer, "CV_X");
  auto ov= CC_CSV(c::Integer, "CV_O");

  auto bd= mc_new_3( Board, nil, xv, ov);
  auto ent= engine->reifyEntity("*");
  auto ps= mc_new( Players);
  ArrCells seed;

  initPlayers(ps);
  seed.fill(nil);

  ent->checkin(mc_new_1( PlayView , MGML()));
  ent->checkin(mc_new( UISelection));
  ent->checkin(mc_new_1( SmartAlgo, bd));
  ent->checkin(ps);
  ent->checkin(mc_new_1( Grid, seed));

  return ent;
};

//////////////////////////////////////////////////////////////////////////
//
void EFactory::initPlayers(Players *ps) {

  auto human = CC_CSV(c::Integer, "HUMAN");
  auto bot = CC_CSV(c::Integer, "BOT");
  auto netp = CC_CSV(c::Integer, "NETP");

  auto vx= CC_CSV(c::Integer, "CV_X");
  auto vo= CC_CSV(c::Integer, "CV_O");
  auto p1c= CC_CSS("P1_COLOR");
  auto p2c= CC_CSS("P2_COLOR");

  auto mode = MGMS()->getMode();
  auto p1cat= human;
  auto p2cat= bot;

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

  ps->parr[1] = Player(p1cat, vx, 1, p1c);
  ps->parr[2] = Player(p2cat, vo, 2, p2c);
  ps->parr[0] = Player();
}


NS_END(tttoe)


