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
struct CC_DLL Aliens : public ecs::System {

  MDECL_SYS_TPID( "n/Aliens")
  MDECL_SYS_PRIORITY(ecs::AI)
  MDECL_SYS_UPDATE()
  MDECL_SYS_PREAMBLE()

  void forwardOneAlien(ecs::Node*, float delta);
  void shuffleOneAlien(ecs::Node*, int stepx);

  void processMovement(float dt);
  void processBombs( float dt);

  void dropBomb(float x, float y);
  void checkBomb(AlienSquad* );

  bool testDirX(ecs::Node* , int stepx);
  void maybeShuffleAliens(AlienSquad* );

  bool doShuffle(AlienSquad* );
  bool doForward(AlienSquad* );

  ecs::Node* findMinX(AlienSquad* );
  ecs::Node* findMaxX(AlienSquad* );

  __decl_ptr(ecs::Node, _baddies)

  Aliens(ecs::Engine *e)
  : System(e)
  {}

};


NS_END

