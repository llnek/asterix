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

#if !defined(__EFACTORY_H__)
#define __EFACTORY_H__

#include "core/XPool.h"
#include "ash/Engine.h"
#include "n/GNodes.h"
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GEngine : public a::Engine {

  void reifyExplosions(int count = 24);
  void reifyMissiles(int count= 36);
  void reifyBombs(int count = 24);

  virtual void initEntities();
  virtual void initSystems();

  a::Entity* reifyAliens();
  a::Entity* reifyShip();
  a::Entity* reifyArena();

private:
  const c::Size getRankInfo(int r, c::Dictionary*);
  void fillSquad(f::XPool* );
};


NS_END(invaders)
#endif


