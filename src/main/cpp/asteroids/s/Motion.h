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

#if !defined(__MOTION_H__)
#define __MOTION_H__

#include "ash/System.h"
#include "GEngine.h"

NS_BEGIN(asteroids)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Motions : public a::System {

  MDECL_SYS_PRIORITY(a::Motion)
  MDECL_SYS_TPID("s/Motions")
  MDECL_SYS_PREAMBLE()
  MDECL_SYS_UPDATE()

  Motions(a::Engine *e)
  : System(e)
  {}

  DECL_PTR(a::NodeList, cannonNode)
  DECL_PTR(a::NodeList, shipNode)
  DECL_PTR(a::NodeList, arenaNode)
  DECL_TD(VOIDFN, rotRight)
  DECL_TD(VOIDFN, rotLeft)
  DECL_TD(VOIDFN, sftUp)
  DECL_TD(VOIDFN, sftDown)

protected:
  void initKeyOps(Motion*, int);
  void controlCannon(float);
  void fireMissile(float);
  void scanInput(float);
};


NS_END(asteroids)
#endif


