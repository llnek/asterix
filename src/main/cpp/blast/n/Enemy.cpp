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
#include "Enemy.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(blast)

//////////////////////////////////////////////////////////////////////////////
//
bool Enemy::init() {

  if (! c::DrawNode::init()) {
  return false; }

  c::Vec2 vertices[NUM_SPIKES*2];
  generateVertices(vertices);

  // draw the star shaped polygon filled with red colour
  drawPolygon(
      vertices, NUM_SPIKES*2,
      c::ccc4f(1,0,0,1), 1.5, c::ccc4f(1,0,0,1));
  // draw a black hole in the middle
  drawDot(CC_ZPT, ENEMY_RADIUS, c::ccc4f(0,0,0,1));

  setScale(0);
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Enemy::generateVertices(c::Vec2 vertices[]) {
  s_vec<c::Vec2> inner_vertices;
  s_vec<c::Vec2> outer_vertices;

  // get two regular polygons, one smaller than the other and with a slightly advance rotation
  getRegularPolygonVertices(inner_vertices, NUM_SPIKES, ENEMY_RADIUS);
  getRegularPolygonVertices(
      outer_vertices, NUM_SPIKES,
      ENEMY_RADIUS * 1.5, M_PI / NUM_SPIKES);

  // run a loop to splice the polygons together to form a star
  for (auto i = 0; i < NUM_SPIKES; ++i) {
    vertices[i*2] = inner_vertices[i];
    vertices[i*2 + 1] = outer_vertices[i];
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Enemy::update(const c::Vec2 &playerpos, bool towards_player) {
  // no movement while spawning
  if (isSpawning) {
  return; }
    auto box=MGMS()->getEnclosureRect();
    auto pt=getPosition();
  // first find a vector pointing to the player
  auto dir= c::ccpSub(playerpos, pt);
  // normalize direction then multiply with the speed_multiplier_
  speed = c::ccpMult(dir.getNormalized(), speedMultiplier * (towards_player ? 1 : -1));

  // restrict movement within the boundary of the game
  auto nextpos= c::ccpAdd(pt, speed);
  if (RECT_CONTAINS_CIRCLE(box, nextpos, ENEMY_RADIUS * 1.5)) {
    setPosition(nextpos);
  }
  else if (RECT_CONTAINS_CIRCLE(box,
        c::Vec2(nextpos.x - speed.x, nextpos.y), ENEMY_RADIUS * 1.5)) {
    setPosition(nextpos.x - speed.x, nextpos.y);
  }
  else if (RECT_CONTAINS_CIRCLE(box,
        c::Vec2(nextpos.x, nextpos.y - speed.y), ENEMY_RADIUS * 1.5)) {
    setPosition(nextpos.x, nextpos.y - speed.y);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Enemy::tick() {
  // no ticking while spawning
  if (isSpawning) {
  return; }

  ++timeAlive;

  // as time increases, so does speed
  switch (timeAlive) {
    case E_SLOW:
      speedMultiplier = 0.5;
    break;
    case E_MEDIUM:
      speedMultiplier = 0.75;
    break;
    case E_FAST:
      speedMultiplier = 1.25;
    break;
    case E_SUPER_FAST:
      speedMultiplier = 1.5;
    break;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Enemy::spawn(float delay) {
  // play a scale-up animation
  isSpawning = true;
  runAction(
      c::Sequence::create(
        c::DelayTime::create(delay),
        c::EaseElasticOut::create(
          c::ScaleTo::create(1, 1)),
        c::CallFunc::create([=]() {
          this->finishSpawn();
          }),
        CC_NIL));
}

//////////////////////////////////////////////////////////////////////////////
//
void Enemy::finishSpawn() {
  isSpawning = false;
}

//////////////////////////////////////////////////////////////////////////////
//
void Enemy::die() {
  // shouldn't die once already dead
  if (isDead) {
  return; }

  // remove this enemy in the next iteration
  mustBeRemoved = true;
  isDead = true;

  // animate death then remove with cleanup
  runAction(
      c::Sequence::create(
        c::Spawn::create(
          c::EaseSineIn::create(
            c::ScaleTo::create(0.1, 0)),
          c::EaseSineIn::create(
            c::RotateBy::create(0.1, -90)), CC_NIL),
        c::RemoveSelf::create(true),
        CC_NIL));

  cx::sfxPlay("enemy_death");
}


NS_END


