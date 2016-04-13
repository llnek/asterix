// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

#include "core/CCSX.h"
#include "Generate.h"
#include "Clear.h"
#include "Move.h"
#include "Resolve.h"
#include "GEngine.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////////
//
void GEngine::initSystems() {
  regoSystem(mc_new1(Generate,this));
  regoSystem(mc_new1(Resolve,this));
  regoSystem(mc_new1(Clear,this));
  regoSystem(mc_new1(Move,this));
}

//////////////////////////////////////////////////////////////////////////
//
void GEngine::initEntities() {
  auto ent= this->reifyNode("Arena", true);

  ent->checkin(mc_new(FilledLines));
  ent->checkin(mc_new(ShapeShell));
  ent->checkin(mc_new(BlockGrid));
  ent->checkin(mc_new(Gesture));
  ent->checkin(mc_new(GVars));
  ent->checkin(mc_new(Dropper));
  ent->checkin(mc_new(Pauser));
  ent->checkin(mc_new(CtrlPad));
}


NS_END


