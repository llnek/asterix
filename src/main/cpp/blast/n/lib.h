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
NS_BEGIN(blast)

#define MAX_STARS 250
#define MAX_ENEMIES 250
#define MAX_POWERUPS 5
#define MAX_BLASTS 25
#define MAX_MISSILES 15

#define PLAYER_RADIUS 20
#define ENEMY_RADIUS 10
#define MISSILE_RADIUS 5
#define POWERUP_ICON_OUTER_RADIUS 20
#define POWERUP_ICON_INNER_RADIUS 10.5

#define ENEMY_SPAWN_DELAY 0.125f
#define MISSILE_SPEED 8
#define SHIELD_DURATION 10
#define MAX_POWERUP_WAIT_ON_SCREEN 30
#define COMBO_TIME 3

#define SHIELD_BLINK_TAG 111

#define CIRCLE_INTERSECTS_CIRCLE(circle1_position, circle1_radius, circle2_position, circle2_radius) (cocos2d::ccpDistance((circle1_position), (circle2_position)) <= ((circle1_radius) + (circle2_radius)))
#define CIRCLE_CONTAINS_POINT(circle_position, circle_radius, point) (cocos2d::ccpDistance((circle_position), (point)) <= (circle_radius))
#define RECT_CONTAINS_CIRCLE(rect, circle_position, circle_radius) \
  ((rect).containsPoint(cocos2d::Vec2((circle_position).x - (circle_radius), (circle_position).y)) && \
  (rect).containsPoint(cocos2d::Vec2((circle_position).x, (circle_position).y + (circle_radius))) && \
  (rect).containsPoint(cocos2d::Vec2((circle_position).x + (circle_radius), (circle_position).y)) && \
  (rect).containsPoint(cocos2d::Vec2((circle_position).x, (circle_position).y - (circle_radius))))

// enum used for proper z-ordering
enum EZIndex {
  E_LAYER_BACKGROUND = 0,
  E_LAYER_FOREGROUND = 2,
  E_LAYER_ENEMIES = 4,
  E_LAYER_BLASTS = 6,
  E_LAYER_MISSILES = 8,
  E_LAYER_POWERUPS = 10,
  E_LAYER_PLAYER = 12,
  E_LAYER_HUD = 14,
  E_LAYER_POPUP = 16
};

enum ESkillTimer {
  E_SKILL1 = 10,
  E_SKILL2 = 30,
  E_SKILL3 = 45,
  E_SKILL4 = 60,
  E_SKILL5 = 90,
  E_SKILL6 = 120
};

enum EEnemySpeedTimer {
  E_SLOW = 5,
  E_MEDIUM = 10,
  E_FAST = 15,
  E_SUPER_FAST = 25
};

enum EEnemyFormation {
  E_FORMATION_RANDOM_EASY = 0,
  E_FORMATION_VERTICAL_EASY,
  E_FORMATION_HORIZONTAL_EASY,
  E_FORMATION_POLYGON_EASY,
  E_FORMATION_RANDOM_MEDIUM,
  E_FORMATION_VERTICAL_MEDIUM,
  E_FORMATION_HORIZONTAL_MEDIUM,
  E_FORMATION_POLYGON_MEDIUM,
  E_FORMATION_RANDOM_HARD,
  E_FORMATION_VERTICAL_HARD,
  E_FORMATION_HORIZONTAL_HARD,
  E_FORMATION_POLYGON_HARD,
  E_FORMATION_MAX//12
};

enum EPowerUpType {
  E_POWERUP_BOMB = 0,
  E_POWERUP_MISSILE_LAUNCHER,
  E_POWERUP_SHIELD,
  E_POWERUP_MAX// 3
};

//////////////////////////////////////////////////////////////////////////////
//

void getRegularPolygonVertices( s_vec<c::Vec2>&vertices, int num_vertices, float circum_radius);

void getRegularPolygonVertices(s_vec<c::Vec2> &vertices, int num_vertices, float circum_radius, float start_angle);

int getNumVerticesForFormation(EEnemyFormation type);

const s_vec<c::Vec2> getEnemyFormation(
    EEnemyFormation type, const c::Rect& boundary, const c::Vec2& anchor_point);

void generateRandomFormation(
    s_vec<c::Vec2> &vertices, int num_vertices, const c::Rect& boundary);

void generateVerticalFormation(
    s_vec<c::Vec2> &vertices, int num_vertices, const c::Rect& boundary);

void generateHorizontalFormation(
    s_vec<c::Vec2> &vertices, int num_vertices, const c::Rect& boundary);

void generatePolygonFormation(
    EEnemyFormation type, s_vec<c::Vec2> &vertices, int num_vertices,
    const c::Rect& boundary, const c::Vec2& anchor_point);







NS_END


