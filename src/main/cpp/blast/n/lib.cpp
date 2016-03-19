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

#include "core/XConfig.h"
#include "core/COMP.h"
#include "core/CCSX.h"
#include "lib.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(blast)

/*
// arrays to store the frequency of formations for each skill level
const int GameGlobals::skill1_formations[] = {0, 4};
const int GameGlobals::skill2_formations[] = {4, 4, 4, 4, 1, 1, 1, 2, 2, 2};
const int GameGlobals::skill3_formations[] = {4, 4, 4, 8, 8, 1, 1, 2, 2, 5, 5, 5, 6, 6, 6, 3, 3};
const int GameGlobals::skill4_formations[] = {4, 4, 8, 8, 8, 5, 5, 5, 6, 6, 6, 3, 3, 3, 7, 7, 7};
const int GameGlobals::skill5_formations[] = {8, 8, 8, 3, 3, 3, 5, 5, 6, 6, 9, 9, 10, 10, 7, 7, 7};
const int GameGlobals::skill6_formations[] = {8, 8, 8, 5, 5, 6, 6, 9, 9, 10, 10, 7, 7, 7, 11, 11, 11};
const int GameGlobals::powerup_frequency[] = {0, 0, 0, 0, 0, 1, 1, 1, 1, 2};
*/

//////////////////////////////////////////////////////////////////////////////
//
void getRegularPolygonVertices(
    s_vec<c::Vec2> &vertices, int num_vertices, float circum_radius) {
  getRegularPolygonVertices(vertices, num_vertices, circum_radius, 0);
}

//////////////////////////////////////////////////////////////////////////////
//
void getRegularPolygonVertices(
    s_vec<c::Vec2> &vertices, int num_vertices, float circum_radius, float start_angle) {

  auto delta_theta = 2 * M_PI / num_vertices;
  auto theta = start_angle;
  vertices.clear();

  for (auto i = 0; i < num_vertices; ++i, theta += delta_theta) {
    vertices.push_back(
        c::Vec2(circum_radius * cosf(theta),
          circum_radius * sinf(theta)));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
int getNumVerticesForFormation(EEnemyFormation type) {

  switch (type) {
    case E_FORMATION_RANDOM_EASY: return 1 + cx::randInt(5);
    case E_FORMATION_RANDOM_MEDIUM: return 5 + cx::randInt(10);
    case E_FORMATION_RANDOM_HARD: return 10 + cx::randInt(15);
    case E_FORMATION_VERTICAL_EASY: return 6;
    case E_FORMATION_VERTICAL_MEDIUM: return 10;
    case E_FORMATION_VERTICAL_HARD: return 14;
    case E_FORMATION_HORIZONTAL_EASY: return 10;
    case E_FORMATION_HORIZONTAL_MEDIUM: return 14;
    case E_FORMATION_HORIZONTAL_HARD: return 20;
    case E_FORMATION_POLYGON_EASY: return 5 + cx::randInt(6);
    case E_FORMATION_POLYGON_MEDIUM: return 2 * (5 + cx::randInt(5));
    case E_FORMATION_POLYGON_HARD: return 3 * (5 + cx::randInt(4));
  }

  return 0;
}

//////////////////////////////////////////////////////////////////////////////
//
const s_vec<c::Vec2> getEnemyFormation(EEnemyFormation type, const c::Rect &boundary, const c::Vec2 &anchor_point) {

  int num_vertices = getNumVerticesForFormation(type);
  s_vec<c::Vec2> vertices;

  switch(type) {
    case E_FORMATION_RANDOM_EASY:
    case E_FORMATION_RANDOM_MEDIUM:
    case E_FORMATION_RANDOM_HARD:
      generateRandomFormation(vertices, num_vertices, boundary);
      break;
    case E_FORMATION_VERTICAL_EASY:
    case E_FORMATION_VERTICAL_MEDIUM:
    case E_FORMATION_VERTICAL_HARD:
      generateVerticalFormation(vertices, num_vertices, boundary);
      break;
    case E_FORMATION_HORIZONTAL_EASY:
    case E_FORMATION_HORIZONTAL_MEDIUM:
    case E_FORMATION_HORIZONTAL_HARD:
      generateHorizontalFormation(vertices, num_vertices, boundary);
      break;
    case E_FORMATION_POLYGON_EASY:
    case E_FORMATION_POLYGON_MEDIUM:
    case E_FORMATION_POLYGON_HARD:
      generatePolygonFormation(type, vertices, num_vertices, boundary, anchor_point);
      break;
  }

  return vertices;
}

//////////////////////////////////////////////////////////////////////////////
//
void generateRandomFormation(s_vec<c::Vec2> &vertices, int num_vertices, const c::Rect &boundary) {

  // return random positions within the game's boundary
  for (auto i = 0; i < num_vertices; ++i) {
      auto vertex = c::Vec2(0,0);
    vertex.x = boundary.origin.x + ENEMY_RADIUS*2 + (cx::rand() * (CC_ZW(boundary.size) - ENEMY_RADIUS*4));
    vertex.y = boundary.origin.y + ENEMY_RADIUS*2 + (cx::rand() * (CC_ZH(boundary.size) - ENEMY_RADIUS*4));
    vertices.push_back(vertex);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void generateVerticalFormation(s_vec<c::Vec2> &vertices, int num_vertices, const c::Rect &boundary) {
  // choose between left & right edge of screen
  auto start_x = (CCRANDOM_MINUS1_1() > 0)
    ? boundary.origin.x + ENEMY_RADIUS*2
    : boundary.origin.x + CC_ZW(boundary.size) - ENEMY_RADIUS*2;

  auto start_point = c::Vec2(start_x, boundary.origin.y + ENEMY_RADIUS*2);
  // calculate vertical distance between adjacent enemies
  auto vertical_gap = CC_ZH(boundary.size) / num_vertices;

  for (auto i = 0; i < num_vertices; ++i) {
    vertices.push_back(c::Vec2(start_point.x, start_point.y + i * vertical_gap));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void generateHorizontalFormation(s_vec<c::Vec2> &vertices, int num_vertices, const c::Rect &boundary) {
  // choose between top & bottom edge of screen
  auto start_y = (CCRANDOM_MINUS1_1() > 0)
    ? boundary.origin.y + ENEMY_RADIUS*2
    : boundary.origin.y + CC_ZH(boundary.size) - ENEMY_RADIUS*2;
  auto start_point = c::Vec2(boundary.origin.x + ENEMY_RADIUS*2, start_y);
  // calculate horizontal distance between adjacent enemies
  auto horizontal_gap = CC_ZW(boundary.size) / num_vertices;

  for (auto i = 0; i < num_vertices; ++i) {
    vertices.push_back(c::Vec2(start_point.x + i * horizontal_gap, start_point.y));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void generatePolygonFormation(
    EEnemyFormation type, s_vec<c::Vec2> &vertices,
    int num_vertices, const c::Rect &boundary, const c::Vec2 &anchor_point) {

  auto wz= cx::visRect();
  int num_polygons = 1;
  auto polygon_radius = CC_ZW(wz.size)/6;
  // formations contain more polygons and are smaller as they increase in difficulty
  switch (type) {
    case E_FORMATION_POLYGON_EASY:
      num_polygons = 1;
      polygon_radius = CC_ZW(wz.size)/6;
      break;
    case E_FORMATION_POLYGON_MEDIUM:
      num_polygons = 2;
      polygon_radius = CC_ZW(wz.size)/7;
      break;
    case E_FORMATION_POLYGON_HARD:
      num_polygons = 3;
      polygon_radius = CC_ZW(wz.size)/8;
      break;
  }

  // calculate number of vertices for a single polygon
  auto num_vertices_per_polygon = (int)(num_vertices/num_polygons);
  s_vec<c::Vec2> vertices_per_polygon;

  for (auto i = 0; i < num_polygons; ++i) {
    // get the vertices of a single polygon
    getRegularPolygonVertices(vertices_per_polygon,
        num_vertices_per_polygon, (i * ENEMY_RADIUS * 2.5 + polygon_radius));

    // copy the vertices into the final array
    for (auto j = 0; j < num_vertices_per_polygon; ++j) {
      // the anchor point will act as the center of the polygon
      auto vertex = c::ccpAdd(vertices_per_polygon[j], anchor_point);
      // exclude any vertices that are out of the boundary
      if (RECT_CONTAINS_CIRCLE(boundary, vertex, ENEMY_RADIUS*2)) {
        vertices.push_back(vertex);
      }
    }

    vertices_per_polygon.clear();
  }
}


NS_END


