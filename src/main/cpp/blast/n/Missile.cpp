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

#include "Blast.h"
#include "Missile.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(blast)

//////////////////////////////////////////////////////////////////////////////
//
owner<Missile*> Missile::create(not_null<GVars*> ss, const CCT_PT &target, const CCT_PT &speed) {
  auto missile = f::reifyRefType<Missile>();
  missile->target = target;
  missile->speed = speed;
  missile->ss=ss;
  return missile;
}

//////////////////////////////////////////////////////////////////////////////
//
bool Missile::init() {
  // generate vertices for the missile
  CCT_PT vertices[] = {
    CCT_PT(MISSILE_RADIUS * 1.75, 0),
    CCT_PT(MISSILE_RADIUS * -0.875, MISSILE_RADIUS),
    CCT_PT(MISSILE_RADIUS * -1.75, 0),
    CCT_PT(MISSILE_RADIUS * -0.875, MISSILE_RADIUS * -1)
  };
  // draw a yellow coloured missile
  drawPolygon(vertices, 4,
      c::ccc4f(0.91765, 1, 0.14118f, 1), 0, c::ccc4f(0, 0, 0, 0));

  schedule(CC_SCHEDULE_SELECTOR(Missile::explode), 5);
  scheduleUpdate();

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Missile::update(float dt) {
  auto pt=getPosition();
  // find a vector pointing to the target
  auto direction = c::ccpSub(target, pt).getNormalized();
  // add the direction to the speed for smooth curved movement
  speed.x += direction.x;
  speed.y += direction.y;
  // normalize the speed & multiply with a constant
  speed = c::ccpMult(speed.getNormalized(), MISSILE_SPEED);

  setPosition(pt.x + speed.x, pt.y + speed.y);

  // update the rotation of the missile
  float angle = c::ccpToAngle(c::ccpSub(pt, prevPos));
  setRotation(CC_RADIANS_TO_DEGREES(angle * -1));
  prevPos= pt;

  // explode the missile if it has roughly reached the target
  if (pt.fuzzyEquals(target, ENEMY_RADIUS * 1.5)) {
    explode();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Missile::explode(float dt) {
  // can't expode more than once
  if (hasExploded) {
  return; }

  auto pt=getPosition();
  hasExploded = true;
  // create three blasts on explosion
  for (auto i = 0; i < 3; ++i) {
    // create a blast twice the size of the player that should last for quarter of a second
    auto blast = Blast::create(PLAYER_RADIUS * 2, 0.25);
    // position it randomly around the missile
    blast->setPosition(c::ccpAdd(pt,
          CCT_PT(cx::randInt(PLAYER_RADIUS * 2 * i),
            cx::randInt(PLAYER_RADIUS * 2 * i))));
    addBlast(ss, blast);
  }
  // remove this missile in the next iteration
  mustBeRemoved = true;
  runAction(
      c::Sequence::create(
        c::DelayTime::create(0.01),
        c::RemoveSelf::create(true),
        CC_NIL));

  cx::sfxPlay("small_blast");
}


NS_END



