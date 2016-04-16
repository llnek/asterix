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

#include "ParticleSpin.h"
#include "2d/CCNode.h"
NS_BEGIN(bazuka)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL ParticleLayer : public c::Node {

  __decl_ptr(ParticleSpin, _enemyDie)
  __decl_ptr(ParticleSpin, _enemyGun)
  void removeSelf(float dt);

public:

  ParticleLayer(const CCT_PT&);
  virtual void update(float dt);

};

NS_END


