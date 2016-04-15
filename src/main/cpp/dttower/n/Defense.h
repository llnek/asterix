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

#include "core/COMP.h"
NS_BEGIN(dttower)

enum DefenseLevel {
  levelOne = 0,
  levelTwo
};

//////////////////////////////////////////////////////////////////////////////
//
class Enemy;
class GVars;
class CC_DLL Defense  : public f::CPixie {

  Defense(GVars *ss, DefenseLevel d, int pts) {
    _attackPoints=pts;
    _defenseLevel=d;
    this->ss = ss;
  }

  __decl_md(DefenseLevel, _defenseLevel)
  __decl_iz(_attackPoints)
  __decl_ptr(GVars, ss)

public:

  static owner<Defense*> create(not_null<GVars*>, DefenseLevel);
  void checkEnemy(not_null<ecs::Node*>, not_null<c::Sprite*>);
  void attackEnemy(not_null<ecs::Node*>);
  bool detectEnemy(not_null<Enemy*>);

  __decl_getr(int, _attackPoints, AttackPoints)
  virtual void update(float);

};


NS_END



