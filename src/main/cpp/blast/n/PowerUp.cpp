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
#include "PowerUp.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(blast)

//////////////////////////////////////////////////////////////////////////////
//
bool PowerUp::init() {

  if (! c::DrawNode::init()) {
  return false; }

  // calculate how much time the power-up should wait on screen before activation
  timeLeft = MAX_POWERUP_WAIT_ON_SCREEN / 2 + cx::randInt(MAX_POWERUP_WAIT_ON_SCREEN / 2);
  // calculate speed
  speed = c::Vec2(CCRANDOM_MINUS1_1() * 2, CCRANDOM_MINUS1_1() * 2);

  // draw the brown coloured ring
  drawDot(CC_ZPT, POWERUP_ICON_OUTER_RADIUS,
      c::ccc4f(0.73725f, 0.5451f, 0, 1));

  drawDot(CC_ZPT, POWERUP_ICON_OUTER_RADIUS - 3, c::ccc4f(0, 0, 0, 1));

  setScale(0);

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void PowerUp::update() {
  auto box= MGMS()->getEnclosureRect();
    auto pt= getPosition();
  // bounce within the boundary
  if (!RECT_CONTAINS_CIRCLE(box, pt, POWERUP_ICON_OUTER_RADIUS)) {
    // bounce off the left & right edge
    if ((pt.x - POWERUP_ICON_OUTER_RADIUS) < box.origin.x ||
        (pt.x + POWERUP_ICON_OUTER_RADIUS) > (box.origin.x + box.size.width) )
      speed.x *= -1;

    // bounce off the top & bottom edge
    if ((pt.y + POWERUP_ICON_OUTER_RADIUS) > (box.origin.y + box.size.height) ||
      (pt.y - POWERUP_ICON_OUTER_RADIUS) < box.origin.y )
      speed.y *= -1;
  }

  setPosition(pt.x + speed.x, pt.y + speed.y);
}

//////////////////////////////////////////////////////////////////////////////
//
void PowerUp::tick() {

  --timeLeft;

  // remove this power-up in the next iteration when it's on-screen time is over
  if (timeLeft < 0) {
    mustBeRemoved = true;
    runAction(
        c::Sequence::create(
          c::EaseBackIn::create(
            c::ScaleTo::create(0.25, 0)),
          c::RemoveSelf::create(true),
          CC_NIL));
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void PowerUp::spawn() {
  // animate the entry
  runAction(c::EaseElasticOut::create(c::ScaleTo::create(1, 1)));
  runAction(
      c::RepeatForever::create(
        c::Sequence::create(
          c::DelayTime::create(5 + cx::randInt(5)),
          c::EaseSineIn::create(
            c::ScaleTo::create(0.1, 1.2)),
          c::EaseSineOut::create(
            c::ScaleTo::create(0.1, 1.0)),
          c::EaseSineIn::create(
            c::ScaleTo::create(0.1, 1.2)),
          c::EaseSineOut::create(
            c::ScaleTo::create(0.1, 1)),
          CC_NIL)));
}

//////////////////////////////////////////////////////////////////////////////
//
void PowerUp::activate() {
  // clear the geometry and stop all actions
  // now the child classes can add their own behaviour
  isActive = true;
  clear();
  stopAllActions();
}

//////////////////////////////////////////////////////////////////////////////
//
void PowerUp::deactivate() {
  // remove this power-up in the next iteration
  runAction(
      c::Sequence::create(
        c::DelayTime::create(0.01),
        c::RemoveSelf::create(true),
        CC_NIL));
  mustBeRemoved = true;
}


NS_END


