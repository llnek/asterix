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

#include "core/Primitives.h"
#include "2d/CCSprite.h"

NS_BEGIN(bazuka)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL ParticleSpin : public c::Sprite {

  ParticleSpin() {
    speed.x= CCRANDOM_MINUS1_1() * 2.0f;
    speed.y= (rand()% 3) + 1;
  }

public:

  static ParticleSpin* create(const sstr&, const c::Vec2&);

  __decl_mv(c::Vec2, gravity, c::Vec2(0,-0.25))
  __decl_fz(spinCounter)
  __decl_md(c::Vec2,speed)

  virtual void update(float dt);

};


NS_END


