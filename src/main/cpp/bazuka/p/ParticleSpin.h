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
#include "core/COMP.h"
#include "2d/CCSprite.h"

NS_BEGIN(bazuka)

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL ParticleSpin : public c::Sprite , public f::CMove {

  __decl_mv(CCT_PT, _gravity, c::Vec2(0,-0.25))
  __decl_fz(_spinCounter)

  ParticleSpin() {
    speed.x= CCRANDOM_MINUS1_1() * 2.0;
    speed.y= (rand()% 3) + 1;
  }

public:

  static owner<ParticleSpin*> create(const sstr&, const CCT_PT&);

  virtual void update(float dt);

};


NS_END


