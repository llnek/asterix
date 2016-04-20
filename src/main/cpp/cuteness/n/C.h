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

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(cuteness)

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Planet : public f::CPixie {
  static owner<Planet*> create() {
    auto z= mc_new(Planet);
    z->initWithSpriteFrameName("planet.png");
    z->autorelease();
    return z;
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Enemy : public f::CPixie {
  static owner<Enemy*> create(const sstr &png) {
    auto z= mc_new(Enemy);
    z->initWithSpriteFrameName(png);
    z->autorelease();
    return z;
  }
};

//////////////////////////////////////////////////////////////////////////////
//
class CC_DLL EnemySquad : public ecs::Component {
  __decl_vec(f::FPool*, _pools)
public:
  __decl_comp_tpid( "n/ESquad" )
  f::FPool* randGet() {
    auto n= cx::randInt(_pools.size());
    return _pools[n];
  }
  void add(f::FPool *p) {
    _pools.push_back(p);
  }
  const s_vec<f::FPool*>& getPools() { return _pools; }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {
  __decl_comp_tpid( "n/GVars" )
  __decl_fz(speedBump)
  __decl_bf(enabled)
};

//////////////////////////////////////////////////////////////////////////////
//
void scanAndSmash(EnemySquad *sq, const CCT_PT &tap);





NS_END




