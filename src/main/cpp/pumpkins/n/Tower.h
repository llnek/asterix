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

NS_BEGIN(pumpkins)

class Enemy;
class GVars;
class CC_DLL Tower : public f::CPixie {

  // these take values straight from the TowerDataSet &
  // TowerData structs
  __decl_iz(_type)
  __decl_md(sstr, _bulletName)
  __decl_bf(_lightning)
  __decl_bf(_rotating)
  __decl_fz(_range)
  __decl_fz(_physicalDamage)
  __decl_fz(_magicalDamage)
  __decl_fz(_speedDamage)
  __decl_fz(_speedDamageDuration)
  __decl_fz(_fireRate)
  __decl_iz(_cost)

  // the level of upgrade the tower is currently at
  __decl_iz(_curLevel)
  __decl_ptr(Enemy, _target)
  __decl_ptr(GVars, ss)
  __decl_ptr(c::Sprite, _base)
  // a node to draw the circular range for this tower
  __decl_ptr(c::DrawNode, _rangeNode)

  bool inix(int type, const CCT_PT&);
  void setProps();
  Tower(GVars *ss) { this->ss = ss; }

public:

  static owner<Tower*> create(not_null<GVars*>, int type, const CCT_PT&);

  GVars *getGVars() { return ss; }
  void updateRotation();
  void update();

  // functions that take care of upgradation & resale
  void upgrade();
  void sell();

  // basic tower behaviour
  void checkForEnemies();
  void setTarget(not_null<Enemy*>);
  void shoot(float dt);
  void shootBullet();
  void shootLightning();

  // show the range for this tower
  void createRangeNode();
  void showRange();

  void setSpriteName(const sstr&);
  void setIsRotating(bool);

  __decl_setr(sstr, _bulletName, BulletName)
  __decl_gsms_is(_lightning, Lightning)
  __decl_getr(float,_range,Range)
  void setRange(float r) { _range = r* TILE_SIZE; }

  __decl_gsms(float, _physicalDamage, PhysicalDamage)
  __decl_gsms(float, _magicalDamage, MagicalDamage)
  __decl_gsms(float, _speedDamage, SpeedDamage)

  __decl_gsms(float, _speedDamageDuration, SpeedDamageDuration)
  __decl_gsms(float, _fireRate, FireRate)
  __decl_gsms(int, _cost, Cost)
  __decl_getr(int, _type, Type)
  __decl_getr(int, _curLevel, Level)

};



NS_END




