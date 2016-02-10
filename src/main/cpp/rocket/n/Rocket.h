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

#include "core/ComObj.h"
NS_BEGIN(rocket)

enum {
  ROTATE_NONE,
  ROTATE_CLOCKWISE,
  ROTATE_COUNTER
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Rocket : public f::ComObj {

  Rocket(not_null<c::Sprite*>);

public:

  DECL_FZ(rotationDamping)
  DECL_FZ(rotationSpring)
  DECL_FZ(targetRotation)
  DECL_FZ(dr)
  DECL_FZ(ar)
  DECL_FZ(vr)

  void setRotationFromVector () {
    targetRotation= CC_RADIANS_TO_DEGREES(atan2(-vel.y, vel.x));
  }

  static Rocket* create();

  DECL_IZ(rotationOrientation)
  DECL_TD(c::Vec2, pivot)
  DECL_FZ(angularSpeed)

  void update(float);
  void reset();
  void select(bool flag);

  bool collidedWithSides();

  MDECL_COMP_TPID("n/Rocket")
};


NS_END


