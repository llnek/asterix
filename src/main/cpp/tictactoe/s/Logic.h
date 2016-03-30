// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Ken Leung. All rights reserved.

#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "ecs/System.h"
#include "GEngine.h"

NS_BEGIN(tttoe)

//////////////////////////////////////////////////////////////////////////////
class CC_DLL Logic : public ecs::System {

  void sync(int pos, int value, Grid*);
  void process(float);

public:

  MDECL_SYS_PRIORITY( ecs::Logic)
  MDECL_SYS_TPID("n/Logic")
  MDECL_SYS_PREAMBLE()
  MDECL_SYS_UPDATE()

  __decl_ptr(c::DelayTime, botTimer)
  __decl_ptr(ecs::Node, robot)
  __decl_ptr(ecs::Node, board)
  __decl_ptr(ecs::Node, arena)

  Logic(ecs::Engine *e)
  : System(e)
  {}

};


NS_END


