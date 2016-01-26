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
#include "GEngine.h"

NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Aliens : public a::System {

  MDECL_SYS_TPID( "n/Aliens")
  MDECL_SYS_PRIORITY(a::AI)
  MDECL_SYS_UPDATE()
  MDECL_SYS_PREAMBLE()

  void forwardOneAlien(f::ComObj*, float delta);
  void shuffleOneAlien(f::ComObj*, int stepx);

  void processMovement(float dt);
  void processBombs( float dt);

  void dropBomb(float x, float y);
  void checkBomb(AlienSquad* );

  bool testDirX(f::ComObj* , int stepx);
  void maybeShuffleAliens(AlienSquad* );

  bool doShuffle(AlienSquad* );
  bool doForward(AlienSquad* );

  f::ComObj* findMinX(AlienSquad* );
  f::ComObj* findMaxX(AlienSquad* );

  DECL_PTR(a::NodeList, baddies)

  Aliens(a::Engine *e)
  : System(e)
  {}
};


NS_END(invaders)

