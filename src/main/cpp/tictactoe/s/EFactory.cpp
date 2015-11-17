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

#include "n/TTTBoard.h"
#include "n/cobjs.h"
#include "EFactory.h"
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////
//
EFactory::EFactory(not_null<a::Engine*> e,
    not_null<c::Dictionary*> options)

  : f::Factory(e, options) {

}

//////////////////////////////////////////////////////////////////////////////
//
a::Entity* EFactory::ReifyArena(not_null<f::XLayer*> layer) {

  auto nil= CC_CSV(c::Integer, "CV_Z");
  auto xv= CC_CSV(c::Integer, "CV_X");
  auto ov= CC_CSV(c::Integer, "CV_O");
  auto ent= engine->ReifyEntity("game");

  auto bd= new TTTBoard<BD_SZ>(nil, xv, ov);
  ArrCells seed;
  seed.fill(nil);

  ent->Checkin(new Grid(seed));
  ent->Checkin(new Board());
  ent->Checkin(new UISelection());
  ent->Checkin(new SmartAlgo(bd));
  ent->Checkin(new NetPlay());
  ent->Checkin(new PlayView(layer));

  return ent;
};

void EFactory::InitPlayers() {
/*
  auto human = CC_CSV(c::Integer, "HUMAN");
  auto bot = CC_CSV(c::Integer, "BOT");
  auto netp = CC_CSV(c::Integer, "NETP");

  auto p1c= CC_CSV(c::Integer, "P1_COLOR");
  auto p2c= CC_CSV(c::Integer, "P2_COLOR");
  auto vx= CC_CSV(c::Integer, "CV_X");
  auto vo= CC_CSV(c::Integer, "CV_O");

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
    //p1cat= human;
  }

  p1= new Player(p1cat, vx, 1, p1c);
  p2= new Player(p2cat, vo, 2, p2c);
*/
}


NS_END(tttoe)


