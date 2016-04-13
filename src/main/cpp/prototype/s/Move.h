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

#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "GEngine.h"

NS_BEGIN(prototype)

//////////////////////////////////////////////////////////////////////////////
class CC_DLL Move : public ecs::System {

  __decl_ptr(ecs::Node, _player)
  __decl_ptr(ecs::Node, _shared)

  void process(float);
  void onKeys(float);

public:

  __decl_sys_priority( ecs::Move)
  __decl_sys_tpid("n/Move")
  __decl_sys_preamble()
  __decl_sys_update()

  Move(ecs::Engine *e)
  : System(e)
  {}

};


NS_END




