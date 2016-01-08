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

#if !defined(__GENGINE_H__)
#define __GENGINE_H__

#include "ash/Engine.h"
#include "n/GNodes.h"

NS_BEGIN(asteroids)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GEngine : public a::Engine {

  virtual void initEntities();
  virtual void initSystems();
  void createMissiles(int count=36);
  void createLasers(int count=36);
  void createShip();
  void bornShip();
  void createAsteroids(int rank);

protected:
  bool maybeOverlap(Ship*);
};


NS_END(asteroids)
#endif


