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

#include "x2d/XNode.h"
#include "n/C.h"

NS_BEGIN(flappy)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL TowerBody {

  TowerBody(const CCT_PT &p) {
    position=p;
  }
  TowerBody() {}

  __decl_ptr(c::Sprite,lowerSprite)
  __decl_ptr(c::Sprite,upperSprite)
  __decl_md(CCT_PT,position)

};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL Tower {

  __decl_md(CCT_SZ, towerSpriteSize)
  __decl_ptr(f::XNode,parentNode)
  __decl_vec(TowerBody, towers)
  __decl_iz(firstTowerIndex)
  __decl_iz(lastTowerIndex)

  void createTower();

public:

  Tower(f::XNode *n) { parentNode=n; }
  void init();

  const CCT_PT getNextTowerPosition();
  void createTower(const CCT_PT&);
  void update(float);
  void repositionTower(int);
  TowerBody& getFrontTower();

};


NS_END




