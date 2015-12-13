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

#include "core/CCSX.h"
#include "EFactory.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////
//
EFactory::EFactory(not_null<a::Engine*> e,
    not_null<c::Dictionary*> options)

  : Factory(e,options) {

}

//////////////////////////////////////////////////////////////////////////
//
EFactory::~EFactory() {
}

//////////////////////////////////////////////////////////////////////////
//
a::Entity* reifyArena() {
  auto ent= engine->reifyEntity("*");
  ent->checkin(mc_new (FilledLines));
  ent->checkin(mc_new(ShapeShell));
  ent->checkin(mc_new(BlockGrid));
  ent->checkin(mc_new(TileGrid));
  ent->checkin(mc_new(Motion));
  ent->checkin(mc_new(Dropper));
  ent->checkin(mc_new(Pauser));
  ent->checkin(mc_new(GridBox));
  ent->checkin(mc_new(CtrlPad));
  return ent;
}


NS_END(tetris)

