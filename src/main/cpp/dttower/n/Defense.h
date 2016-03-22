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

  DECL_TD(DefenseLevel, defenseLevel)
  DECL_IZ(attackPoints)
  DECL_PTR(Enemy, enemyInRange)
  DECL_PTR(GVars, ss)

  static owner<Defense*> create(GVars *ss, DefenseLevel, const c::Vec2&);
  virtual void update(float);
    void attackEnemy(float);
    void enemyOutOfRange();
  void enemyKilled();
    bool detectEnemyWithDefenseAtPos(
                                     const c::Vec2 &pos,
                                     float defenseRadius,
                                     const c::Vec2 &enemyPosition,
                                     float enemyRadius);

};


NS_END



