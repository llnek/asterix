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
  DECL_FZ(range)
  DECL_FZ(damage)
  DECL_FZ(damageRate)
  DECL_FZ(lastDamageTime)
  DECL_TD(sstr,sound)

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
  MDECL_COMP_TPID("n/Melee")
  DECL_BF(aoe)
  DECL_TD(sstr,sound)

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Monster : public ecs::Component {
  MDECL_COMP_TPID("n/Monster")
  DECL_TD(MonsterType, type)

  Monster(MonsterType t) {
    this->type = t;
  }

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Stash : public f::CStash {
  MDECL_COMP_TPID("n/Stash")
  DECL_IZ(coins)
  DECL_FZ(lastCoinDrop)

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Team : public ecs::Component {
  MDECL_COMP_TPID("n/Team")
  DECL_IZ(team)

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
    DECL_TV(sstr, _name, "?");
public:
    virtual void update(ecs::Entity*,AILogic*) {}
    virtual void enter() {}
    virtual void exit() {}
    const sstr name() { return _name; }
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Automa : public f::CAutoma {
  MDECL_COMP_TPID("n/Automa")
  DECL_PTR(AIState, state)

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







