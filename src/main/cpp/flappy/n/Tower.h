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

#include "x2d/XNode.h"
#include "n/lib.h"

NS_BEGIN(flappy)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL TowerBody {

  TowerBody(const c::Vec2 &p) {
    position=p;
  }
  TowerBody() {}

  __decl_ptr(c::Sprite,lowerSprite)
  __decl_ptr(c::Sprite,upperSprite)
  __decl_md(c::Vec2,position)

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Tower {

  void createTower();

  __decl_mv(c::Size, towerSpriteSize, c::Size(0,0))
  __decl_ptr(f::XNode,parentNode)
  s_vec<TowerBody> towers;
  __decl_iz(firstTowerIndex)
  __decl_iz(lastTowerIndex)

public:

  Tower(f::XNode *n) { parentNode=n; }
  void init();

  void createTower(const c::Vec2&);
  c::Vec2 getNextTowerPosition();
  void update(float);
  void repositionTower(int);
  TowerBody& getFrontTower();

};


NS_END




