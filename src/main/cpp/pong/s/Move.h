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

NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Move : public ecs::System {

  void simuMove(ecs::Node*, float);
  void moveRobot(ecs::Node*,  float);
  void processBall( float);
  void doit(ecs::Node*, float);
  void notifyServer(ecs::Node*, int);
  void clamp(c::Node*);
    void process(float);
    
public:

  __decl_sys_priority( ecs::Move)
  __decl_sys_tpid( "s/Move")
  __decl_sys_preamble()
  __decl_sys_update()

  Move(ecs::Engine *e)
  : System(e)
  {}

  __decl_ptr(ecs::Node, _arena)
  __decl_ptr(ecs::Node, _ball)
  s_vec<ecs::Node*> _paddles;
};


NS_END



