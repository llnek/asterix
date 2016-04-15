// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

#pragma once
//////////////////////////////////////////////////////////////////////////////

#include "Box2D/Box2D.h"
#include "ecs/Engine.h"
#include "n/Platform.h"
#include "n/GSwitch.h"
#include "n/Igloo.h"
#include "n/Eskimo.h"
#include "n/C.h"

NS_BEGIN(eskimo)
//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL GEngine  : public ecs::Engine {

  __decl_ptr(b2World, _world)

public:

  virtual void initEntities();
  virtual void initSystems();

  GEngine(not_null<b2World*> w) {
    _world=w;
  }
};



NS_END



