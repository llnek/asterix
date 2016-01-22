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

#include "ash/Engine.h"
#include "n/N.h"

NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL GEngine : public a::Engine {

  void createArena();
  void createShip();

public:

  virtual void initEntities();
  virtual void initSystems();

  void createMissiles(int count = 36);
  void createBombs(int count = 36);
  void createExplosions(int count = 6);
  void createHitEffects(int count = 10);
  void createSparks(int count = 6);
  void createEnemies(int count = 3);
  void createBackSkies();
  void createBackTiles(int count= 1);

};


NS_END(terra)


