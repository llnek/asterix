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
#include "Stage.h"
#include "Generate.h"
#include "Clear.h"
#include "Move.h"
#include "Resolve.h"
#include "EFactory.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {
  reifyArena();
}

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initSystems() {

}

//////////////////////////////////////////////////////////////////////////
//
void GEngine::reifyArena() {
  auto ent= this->reifyEntity("*");
  ent->checkin(mc_new_1(FilledLines,this));
  ent->checkin(mc_new_1(ShapeShell,this));
  ent->checkin(mc_new_1(BlockGrid,this));
  ent->checkin(mc_new_1(TileGrid,this));
  ent->checkin(mc_new_1(GVars,this));
  ent->checkin(mc_new_1(Dropper,this));
  ent->checkin(mc_new_1(Pauser,this));
  ent->checkin(mc_new_1(GridBox,this));
  ent->checkin(mc_new_1(CtrlPad,this));
}

NS_END(tetris)

