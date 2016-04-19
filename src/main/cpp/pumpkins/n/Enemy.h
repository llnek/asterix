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
NS_BEGIN(pumpkins)

class GVars;
//////////////////////////////////////////////////////////////////////////////
class CC_DLL Enemy: public f::CPixie {

  __decl_iz(_magicResistance)
  __decl_md(sstr, _animation)
  __decl_iz(_type)
  __decl_iz(_health)
  __decl_iz(_armor)
  __decl_fz(_speed)
  __decl_iz(_damage)
  __decl_iz(_reward)

  __decl_md(CCT_SZ, _size)
  __decl_iz(_healthLeft)
  __decl_bf(_hasDied)
  __decl_bf(_isSlowed)
  __decl_iz(_numWalkPoints)
  __decl_iz(_currWalkPoint)

  __decl_vec(CCT_PT, _walkPoints)
  __decl_ptr(c::ProgressTimer, _healthBar)
  __decl_ptr(GVars, ss)

  bool inix(int type);
  void setProps();
  void createHealthBar();

  Enemy(GVars *ss) { this->ss =ss; }

public:

  static owner<Enemy*> create(not_null<GVars*>, int type);

  virtual float radius();

  GVars* getGVars() { return ss; }
  void updateHealthBar();
  void startWalking();
  void finishWalking();
  void doDamage();
  void takeDamage(not_null<c::Node*>);
  void die();
  void takeSpeedDamage(float damage, float duration);
  void resetSpeed(float dt);

  __decl_gsms(sstr, _animation, AnimationName)

  __decl_gsms(int,_health,Health)
  __decl_gsms(int,_armor,Armor)
  __decl_gsms(int,_magicResistance,MagicResistance)
  __decl_gsms(int,_speed,Speed)
  __decl_gsms(int,_damage,Damage)
  __decl_gsms(int,_reward,Reward)

  __decl_getr(bool, _isSlowed,IsSlowed)
  __decl_getr(bool,_hasDied,HasDied)

  void setWalkPoints(int num_walk_points, const s_vec<CCT_PT>&) {
    _numWalkPoints = num_walk_points;
    _walkPoints = walk_points;
  }

};


NS_END


