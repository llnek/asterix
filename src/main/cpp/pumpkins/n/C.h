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

#define TILE_SIZE 90
#define MAX_COLS 15
#define MAX_ROWS 10

#define GET_X_FOR_COL(col) ( (col) * TILE_SIZE )
#define GET_Y_FOR_ROW(row, h) ( ( (h) - (row) ) * TILE_SIZE )
#define GET_COL_FOR_X(x) ( floor( (x) / TILE_SIZE ) )
#define GET_ROW_FOR_Y(y, h) ( (h) - ceil( (y) / TILE_SIZE ) )

#define NUM_TOWER_UPGRADES 3
#define TOWER_GID 100
#define ENEMY_MOVE_DURATION 1.0f
#define BULLET_MOVE_DURATION 0.15f
#define ENEMY_MOVE_ACTION_TAG 101
#define ENEMY_ANIMATE_ACTION_TAG 201
#define LIGHTNING_KEY_POINT_DIST 15
#define LIGHTNING_DURATION 0.25f
#define MIN_GESTURE_DISTANCE 10

// enum used for proper z-ordering
enum EZorder {
  E_LAYER_BACKGROUND = 0,
  E_LAYER_ENEMY = 2,
  E_LAYER_TOWER = 4,
  E_LAYER_HUD = 6,
  E_LAYER_POPUP = 8
};

enum EGestureType {
  E_GESTURE_NONE = 0,
  E_GESTURE_TAP,
  E_GESTURE_SWIPE_UP,
  E_GESTURE_SWIPE_DOWN,
  E_GESTURE_SWIPE_LEFT,
  E_GESTURE_SWIPE_RIGHT
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL TowerData {

  __decl_fz(speedDamageDuration)
  __decl_md(sstr, sprite)
  __decl_fz(range)
  __decl_fz(physicalDamage)
  __decl_fz(magicalDamage)
  __decl_fz(speedDamage)
  __decl_fz(fireRate)
  __decl_iz(cost)

};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL TowerDataSet {
  __decl_vec(TowerData, towerData)
  __decl_md(sstr, bullet)
  __decl_bf(lightning)
  __decl_bf(rotating)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL EnemyData {
  __decl_md(sstr, animation)
  __decl_iz(magicResistance)
  __decl_iz(health)
  __decl_iz(armor)
  __decl_fz(speed)
  __decl_iz(damage)
  __decl_iz(reward)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL Wave {
  __decl_iz(numEnemies)
  __decl_iz(numEnemiesSpawned)
  __decl_iz(numEnemiesWalking)
  __decl_vec(Enemy*, enemies)
  __decl_fz(spawnDelay)
};

//////////////////////////////////////////////////////////////////////////////
//
struct CC_DLL GVars : public ecs::Component {
  __decl_comp_tpid( "n/GVars" )
  __decl_iz(levelNumber)
  // tower & enemy data libraries
  __decl_iz(numTowerDataSets)
  __decl_vec(TowerDataSet, towerDataSets)
  __decl_iz(numEnemyData)
  __decl_vec(EnemyData, enemyData)

};

//////////////////////////////////////////////////////////////////////////////
//
void scaleLabel(c::Label*);
// function takes comma separated string & returns vector of values
s_vec<int> getIntListFromString(const sstr&);





NS_END




