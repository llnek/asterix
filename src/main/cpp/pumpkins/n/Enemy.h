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

  __decl_md(sstr, _animation)
  __decl_iz(_type)
  __decl_iz(_health)
  __decl_iz(_armor)
  __decl_iz(_magicResistance)
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
  Enemy() {}

public:

  static owner<Enemy*> create(int type);

  virtual float radius();

  void setProperties();
  void createHealthBar();
  void updateHealthBar();

  void startWalking();
  void finishWalking();
  void doDamage();
  void takeDamage(CCObject* object);
  void die();
  void takeSpeedDamage(float speed_damage, float speed_damage_duration);
  void resetSpeed(float dt);

  void setAnimationName(const char* animation_name) { animation_name_ = animation_name; }

  void setHealth(int health) { health_ = health; }
  int getHealth() { return health_; }

  void setArmor(int armor) { armor_ = armor; }
  int getArmor() { return armor_; }

  void setMagicResistance(int magic_resistance) { magic_resistance_ = magic_resistance; }
  int getMagicResistance() { return magic_resistance_; }

  void setSpeed(float speed) { speed_ = speed; }
  float getSpeed() { return speed_; }

  void setDamage(int damage) { damage_ = damage; }
  int getDamage() { return damage_; }

  void setReward(int reward) { reward_ = reward; }
  int getReward() { return reward_; }

  float getRadius() { return radius_; }
  bool getHasDied() { return has_died_; }
  bool getIsSlowed() { return is_slowed_; }

  void setWalkPoints(int num_walk_points, vector<CCPoint> walk_points) { num_walk_points_ = num_walk_points; walk_points_ = walk_points; }

};


NS_END


