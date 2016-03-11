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

#include "ecs/Engine.h"
#include "n/C.h"
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////
//
class CC_DLL GEngine : public ecs::Engine {

  const c::Size getRankInfo(int r, c::Dictionary*);
  void fillSquad(f::FPool* );

public:

  void reifyExplosions(int count = 24);
  void reifyMissiles(int count= 36);
  void reifyBombs(int count = 24);

  virtual void initEntities();
  virtual void initSystems();

  ecs::Node* reifyAliens();
  ecs::Node* reifyShip();
  ecs::Node* reifyArena();

};


NS_END


