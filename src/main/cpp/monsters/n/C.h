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
NS_BEGIN(monsters)

enum MonsterType {
  eMonsterTypeQuirk = 0,
  eMonsterTypeZap,
  eMonsterTypeMunch
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Gun : public ecs::Component {
  MDECL_COMP_TPID("n/Gun")
  __decl_fz(lastDamageTime)
  __decl_fz(range)
  __decl_fz(damage)
  __decl_fz(damageRate)
  __decl_td(sstr,sound)

  Gun(float range, float damage,
      float damageRate,
      const sstr &sound) {
    this->damageRate = damageRate;
    this->range = range;
    this->damage = damage;
    this->sound = sound;
  }

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Melee : public f::CMelee {
  __decl_bf(aoe)
  __decl_td(sstr,sound)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Monster : public ecs::Component {
  MDECL_COMP_TPID("n/Monster")
  __decl_td(MonsterType, type)

  Monster(MonsterType t) {
    this->type = t;
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Player : public f::CStats {
  MDECL_COMP_TPID("n/Player")
  __decl_bf(attacking)
  __decl_iz(coins)
  long long lastCoinDrop;
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Team : public ecs::Component {
  MDECL_COMP_TPID("n/Team")
  __decl_iz(team)

  Team(int team) {
    this->team = team;
  }
};

//////////////////////////////////////////////////////////////////////////////
//
class AILogic;
class Entity;
class CC_DLL AIState {
protected:
  __decl_tv(sstr, _name, "?");
public:
  virtual void update(ecs::Node*,AILogic*) {}
  virtual void enter() {}
  virtual void exit() {}
  const sstr name() { return _name; }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Automa : public f::CAutoma {
  __decl_ptr(AIState, state)

  virtual ~Automa() { mc_del_ptr(state); }

  Automa(not_null<AIState*> s) {
    state = s.get();
  }

  void replaceState(AIState *ns) {
    state->exit();
    delete state;
    state=ns;
    ns->enter();
  }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {
  MDECL_COMP_TPID( "n/GVars" )

};



NS_END







