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
class Defense;
struct CC_DLL Defense  : public f::CPixie {

  __decl_md(DefenseLevel, defenseLevel)
  __decl_iz(attackPoints)
  __decl_ptr(GVars, ss)

  static owner<Defense*> create(GVars *ss, DefenseLevel);
  virtual void update(float);
  bool detectEnemy(Enemy*);
  void checkEnemy(ecs::Node *node, c::Sprite *bullet);
  void attackEnemy(ecs::Node *node);

};


NS_END



