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

#include "ash/System.h"
#include "n/N.h"

NS_BEGIN(breakout)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Collide : public a::System {

  bool onPlayerKilled();
  void check();
  void checkNodes();
  void checkBricks();
  void onBrick(Brick*);

public:

  MDECL_SYS_PRIORITY(a::Collide)
  MDECL_SYS_TPID("s/Collide")
  MDECL_SYS_UPDATE()
  MDECL_SYS_PREAMBLE()

  Collide(a::Engine *e)
  : System(e)
  {}

  DECL_PTR(a::NodeList, paddle)
  DECL_PTR(a::NodeList, ball)
  DECL_PTR(a::NodeList, fence)

};


NS_END(breakout)


