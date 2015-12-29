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

#include "ash/Engine.h"
#include "n/CObjs.h"

NS_BEGIN(pong)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GEngine : public a::Engine {

  a::Entity* mkOnePaddle(int cur, const Player&, float x, float y);
  a::Entity* mkBall(float x, float y);
  a::Entity* mkArena(int cur);

  virtual void ignite();
  virtual ~GEngine() {}
  GEngine() {}
  NOCPYASS(GEngine)
};

NS_END(pong)
#endif


