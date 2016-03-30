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

NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL Motions : public ecs::System {

  void processAlienMotions(float dt);
  void processCannon( float dt);

  __decl_ptr(ecs::Node, _cannon)
  __decl_ptr(ecs::Node, _aliens)

public:

  MDECL_SYS_PRIORITY( ecs::Motion)
  MDECL_SYS_TPID( "n/Motions")
  MDECL_SYS_PREAMBLE()
  MDECL_SYS_UPDATE()

  Motions(ecs::Engine *e)
  : System(e) {}

};


NS_END


