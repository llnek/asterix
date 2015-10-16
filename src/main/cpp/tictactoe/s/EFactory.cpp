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
  auto dx = new f::FArray<int>(size);
  auto dy = new f::FArray<int>(size);

  for (int r=0; r < size; ++r) {
    auto h = new f::FArray<int>(size);
    auto v = new f::FArray<int>(size);
    for (int c=0; c < size; ++c) {
      h->Set(c, r * size + c);
      v->Set(c, c * size + r);
    }
    goals.push_back(h);
    goals.push_back(v);
    dx->Set(r, r * size + r);
    dy->Set(r, (size - r - 1) * size + r);
  }

  goals.push_back(dx);
  goals.push_back(dy);
}

//////////////////////////////////////////////////////////////////////////
//
EFactory::EFactory(not_null<a::Engine*> e,
    not_null<c::Dictionary*> options)
: Factory(e, options) {
  auto size= f::DictVal<c::Integer>(options, "size");
  MapGoalSpace(size->getValue());
}

//////////////////////////////////////////////////////////////////////////
//
EFactory::~EFactory() {
  for (auto it= goals.begin(); it != goals.end(); ++it) {
     delete *it;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
a::Entity* EFactory::ReifyBoard(c::Node* layer) {
  auto size= f::DictVal<c::Integer>(options, "size")->getValue();
  auto nil= f::CstVal<c::Integer>("CV_Z")->getValue();
  auto xv= f::CstVal<c::Integer>("CV_X")->getValue();
  auto ov= f::CstVal<c::Integer>("CV_O")->getValue();
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


