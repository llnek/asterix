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

#if !defined(__MOVE_H__)
#define __MOVE_H__

#include "core/XPool.h"
#include "ash/System.h"
#include "GEngine.h"
NS_BEGIN(asteroids)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Move : public a::System {

  MDECL_SYS_PRIORITY(a::Move)
  MDECL_SYS_TPID("s/Move")
  MDECL_SYS_PREAMBLE()
  MDECL_SYS_UPDATE()

  Move(a::Engine *e)
  : System(e)
  {}

  DECL_PTR(a::NodeList, arenaNode)
  DECL_PTR(a::NodeList, shipNode)

protected:

  const c::Vec2 thrust(float angle, float power);
  float rotateShip(float cur, float deg);
  void processShipMotions(float dt);
  void moveShip(float);
  void moveAstros(f::ComObj*, float);
    void moveXXX(f::XPool*, float);
    void moveBBB(f::XPool*, float);
};


NS_END(asteroids)
#endif


