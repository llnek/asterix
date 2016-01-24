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
#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "ash/System.h"
#include "GEngine.h"
NS_BEGIN(asteroids)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Motions : public a::System {

  void controlCannon(float);
  void fireMissile(float);
  void scanInput(Gesture*);

  DECL_TD(VOIDFN, scanner)

public:

  MDECL_SYS_PRIORITY(a::Motion)
  MDECL_SYS_TPID("s/Motions")
  MDECL_SYS_PREAMBLE()
  MDECL_SYS_UPDATE()

  Motions(a::Engine *e)
  : System(e)
  {}

  DECL_PTR(a::NodeList, cannon)
  DECL_PTR(a::NodeList, ship)
  DECL_PTR(a::NodeList, arena)

};


NS_END(asteroids)


