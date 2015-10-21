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
NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
//
void EFactory::MapGoalSpace(int size) {

  f::FArray<int> dx(size);
  f::FArray<int> dy(size);

  for (int r=0; r < size; ++r) {
    f::FArray<int> h(size);
    f::FArray<int> v(size);
    for (int c=0; c < size; ++c) {
      h.Set(c, r * size + c);
      v.Set(c, c * size + r);
    }
    goals.push_back(h);
    goals.push_back(v);
    dx.Set(r, r * size + r);
    dy.Set(r, (size - r - 1) * size + r);
  }

  goals.push_back(dx);
  goals.push_back(dy);
}

//////////////////////////////////////////////////////////////////////////
//
EFactory::EFactory(not_null<a::Engine*> e,
    not_null<c::Dictionary*> options)

  : f::Factory(e, options) {

  auto size= CC_GDV(c::Integer, options, "size");
  MapGoalSpace(size);
}

//////////////////////////////////////////////////////////////////////////////
//
a::Entity* EFactory::ReifyBoard(not_null<c::Node*> layer) {

  auto size= CC_GDV(c::Integer, state, "size");
  auto nil= CC_CSV(c::Integer, "CV_Z");
  auto xv= CC_CSV(c::Integer, "CV_X");
  auto ov= CC_CSV(c::Integer, "CV_O");
  auto ent= engine->ReifyEntity("game");

  f::FArray<int> seed(size * size);
  seed.Fill(nil);

  auto bd= new TTTBoard(size, nil, xv, ov, goals);
  ent->Checkin(new Grid(size, seed));
  ent->Checkin(new Board(size, goals));
  ent->Checkin(new UISelection());
  ent->Checkin(new SmartAlgo(bd));
  ent->Checkin(new NetPlay());
  ent->Checkin(new GridView(size, layer));

  return ent;
};



NS_END(tttoe)


