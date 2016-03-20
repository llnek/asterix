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

#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"
#include "lib.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(blast)

//////////////////////////////////////////////////////////////////////////////
//
class Shield;
class Player : public f::CDrawNode {
protected:

  DECL_PTR(Shield, _shield)
    Player() {}
    
public:

  static owner<Player*> create() {
    auto z= new Player();
    z->init();
    z->autorelease();
    return z;
  }

  virtual void update(float dt);
  virtual bool init();

  void updateRotation(const c::Vec2& prev);
  void updatePosition();
  void dead();
  void die();

  Shield* getShield() { return _shield; }
  void setShield(Shield*);

  DECL_TV(float, radius,PLAYER_RADIUS)
  DECL_TV(c::Vec2, speed, CC_ZPT)
  DECL_BF(isDying)

};


NS_END


