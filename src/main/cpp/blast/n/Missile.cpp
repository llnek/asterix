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
#include "Blast.h"
#include "Missile.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(blast)

//////////////////////////////////////////////////////////////////////////////
//
owner<Missile*> Missile::create(const c::Vec2 &target, const c::Vec2 &speed) {
  auto missile = f::reifyRefType<Missile>();
  missile->_target = target;
  missile->_speed = speed;
  // generate vertices for the missile
  c::Vec2 vertices[] = {
    c::Vec2(MISSILE_RADIUS * 1.75, 0),
    c::Vec2(MISSILE_RADIUS * -0.875, MISSILE_RADIUS),
    c::Vec2(MISSILE_RADIUS * -1.75, 0),
    c::Vec2(MISSILE_RADIUS * -0.875, MISSILE_RADIUS * -1)
  };
  // draw a yellow coloured missile
  drawPolygon(vertices, 4,
      c::ccc4f(0.91765, 1, 0.14118f, 1), 0, c::ccc4f(0, 0, 0, 0));

  // schedule to explode after 5 seconds
  scheduleOnce(schedule_selector(Missile::Explode), 5.0f);
  scheduleUpdate();
  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Missile::update(float dt) {
  // find a vector pointing to the target
  auto direction = c::ccpSub(_target, m_obPosition).normalize();
  // add the direction to the speed for smooth curved movement
  _speed.x += direction.x;
  _speed.y += direction.y;
  // normalize the speed & multiply with a constant
  _speed = c::ccpMult(_speed.normalize(), MISSILE_SPEED);

  setPosition(m_obPosition.x + _speed.x, m_obPosition.y + _speed.y);

  // update the rotation of the missile
  float angle = c::ccpToAngle(c::ccpSub(m_obPosition, prevPos));
  setRotation(CC_RADIANS_TO_DEGREES(angle * -1));
  prevPos= m_obPosition;

  // explode the missile if it has roughly reached the target
  if (m_obPosition.fuzzyEquals(_target, ENEMY_RADIUS * 1.5)) {
    explode();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Missile::explode(float dt) {
  // can't expode more than once
  if (_has_exploded) {
  return; }

  _has_exploded = true;
  // create three blasts on explosion
  for (auto i = 0; i < 3; ++i) {
    // create a blast twice the size of the player that should last for quarter of a second
    auto blast = Blast::create(PLAYER_RADIUS * 2, 0.25f);
    // position it randomly around the missile
    blast->setPosition(c::ccpAdd(m_obPosition,
          c::Vec2(cx::randInt(PLAYER_RADIUS * 2 * i),
            cx::randInt(PLAYER_RADIUS * 2 * i))));
    game_world_->AddBlast(blast);
  }
  // remove this missile in the next iteration
  _must_be_removed_ = true;
  runAction(
      c::Sequence::create(
        c::DelayTime::create(0.01),
        c::RemoveSelf::create(true),
        CC_NIL));

  cx::sfxPlay("small_blast");
}


NS_END



