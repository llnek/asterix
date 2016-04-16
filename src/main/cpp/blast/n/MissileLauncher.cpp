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

#include "x2d/GameScene.h"
#include "Missile.h"
#include "Player.h"
#include "Enemy.h"
#include "MissileLauncher.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(blast)

//////////////////////////////////////////////////////////////////////////////
//
bool MissileLauncher::init() {

  if (!PowerUp::init()) {
  return false; }

  s_vec<CCT_PT> vertices1;
  s_vec<CCT_PT> vertices2;
  s_vec<CCT_PT> vertices;

  // get two regular pentagons, one smaller than the other and with a slightly advance rotation
  getRegularPolygonVertices(vertices1, 5, POWERUP_ICON_INNER_RADIUS - 6, M_PI * -2/20);
  getRegularPolygonVertices(vertices2, 5, POWERUP_ICON_INNER_RADIUS, M_PI * 2/20);

  // run a loop to splice the pentagons together to form a star
  for (auto i = 0; i < 5; ++i) {
    vertices.push_back(vertices1[i]);
    vertices.push_back(vertices2[i]);
  }

  // draw the star shaped polygon with yellow border
  drawPolygon(&vertices[0], 10,
      c::ccc4f(0, 0, 0, 0), 2,
      c::ccc4f(0.88235, 0.96078, 0, 1));

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void MissileLauncher::update() {
  if (! isActive) {
    PowerUp::update();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void MissileLauncher::activate() {
  if (isActive) {
  return; }

  PowerUp::activate();

  // generate a target for each missile
  s_vec<CCT_PT> initial_direction;
  auto target = generateTargets();
  auto wz= cx::visSize();
  auto pt=getPosition();
  // generate an initial direction vertor for each missile
  getRegularPolygonVertices(initial_direction, 5, CC_ZW(wz)/4, M_PI * 2/20);

  for (auto i = 0; i < 5; ++i) {
    // create a missile with a target, initial direction & speed
    auto missile = Missile::create(ss, target[i],
        c::ccpMult(initial_direction[i].getNormalized(), MISSILE_SPEED));
    // position the missile over the launcher
    CC_POS1(missile, pt);
    addMissile(ss, missile);
  }

  cx::sfxPlay("missile");
  PowerUp::deactivate();
}

//////////////////////////////////////////////////////////////////////////////
//
const s_vec<CCT_PT> MissileLauncher::generateTargets() {
  auto num_enemies = ss->enemies->count();
  auto box=MGMS()->getEnclosureRect();
  s_vec<CCT_PT> target_points;
  auto targets_found = 0;

  // loop through the first 5 enemies within GameWorld & save their positions
  for (auto i = 0; i < num_enemies && targets_found < 5; ++i) {
    auto enemy = (Enemy*)ss->enemies->objectAtIndex(i);
    target_points.push_back(enemy->getPosition());
    ++targets_found;
  }

  // if less than 5 enemies were found, fill up with random positions within the boundary
  while (targets_found < 5) {
    target_points.push_back(
        CCT_PT(cx::randInt(box.origin.x + box.size.width),
        cx::randInt(box.origin.y + box.size.height)));
    ++targets_found;
  }

  return target_points;
}


NS_END


