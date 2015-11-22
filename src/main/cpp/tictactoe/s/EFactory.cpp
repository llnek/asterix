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

#include "EFactory.h"
#include "n/Board.h"
#include "n/CObjs.h"
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////
//
EFactory::EFactory(not_null<a::Engine*> e,
    not_null<c::Dictionary*> options)

  : f::Factory(e, options) {

}

//////////////////////////////////////////////////////////////////////////////
//
a::Entity* EFactory::reifyBoard(not_null<f::XLayer*> layer) {

  auto ent= engine->reifyEntity("game");
  auto nil= CC_CSV(c::Integer, "CV_Z");
  auto xv= CC_CSV(c::Integer, "CV_X");
  auto ov= CC_CSV(c::Integer, "CV_O");

  auto bd= new Board<BD_SZ>(nil, xv, ov);
  auto ps= new Players();
  ArrCells seed;
  seed.fill(nil);

  ent->checkin(new PlayView(layer));
  ent->checkin(new UISelection());
  ent->checkin(new SmartAlgo(bd));
  initPlayers(ps);
  ent->checkin(new Grid(seed));

  return ent;
};

//////////////////////////////////////////////////////////////////////////
//
void EFactory::initPlayers(Players* ps) {

  auto human = CC_CSV(c::Integer, "HUMAN");
  auto bot = CC_CSV(c::Integer, "BOT");
  auto netp = CC_CSV(c::Integer, "NETP");

  auto p1c= CC_CSS("P1_COLOR");
  auto p2c= CC_CSS("P2_COLOR");
  auto vx= CC_CSV(c::Integer, "CV_X");
  auto vo= CC_CSV(c::Integer, "CV_O");

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


