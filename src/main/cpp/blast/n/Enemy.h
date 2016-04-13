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

#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"
#include "lib.h"

#define NUM_SPIKES 10

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(blast)

//////////////////////////////////////////////////////////////////////////////
//
class Enemy : public f::CDrawNode {
protected:

  void generateVertices(c::Vec2 vertices[]);
  __decl_mv(float, speedMultiplier,0.25)
  __decl_iz(timeAlive)

public:

  static owner<Enemy*> create() {
    auto z= new Enemy();
    z->init();
    z->autorelease();
    return z;
  }

  virtual bool init();

  void update(const c::Vec2 &player, bool towards_player);
  void tick();
  void spawn(float delay);
  void finishSpawn();
  void die();

  __decl_mv(c::Vec2, speed, CC_ZPT)
  __decl_bf(isSpawning)
  __decl_bf(isDead)
  __decl_bf(mustBeRemoved)

};



NS_END



