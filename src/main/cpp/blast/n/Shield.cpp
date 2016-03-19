
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
#include "Shield.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(blast)


//////////////////////////////////////////////////////////////////////////////
//
bool Shield::init() {

  if ( !PowerUp::init()) {
  return false; }

  // generate vertices for a hexagon
  s_vec<c::Vec2> vertices;
  getRegularPolygonVertices(vertices, 6, POWERUP_ICON_INNER_RADIUS, M_PI/6);
  // draw a hexagon with cyan border
  drawPolygon(
      &vertices[0], 6,
      c::ccc4f(0,0,0,0), 3, c::ccc4f(0, 0.96862, 1, 1));

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Shield::update() {

  if (! _is_active) {
    PowerUp::update();
  } else {
    // after activation, shield will follow the player
    setPosition(_ship->getPosition());
    setRotation(_ship->getRotation());
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Shield::tick() {

  if (_is_active) {
    --_shield_time_left;

    // deactivate the shield when it's time is over
    if (_shield_time_left <= 0) {
      deactivate();
    }
    // start blinking the shield when there are just two seconds left
    else if( _shield_time_left == 2) {
      auto blink = c::Blink::create(2, 8);
      blink->setTag(SHIELD_BLINK_TAG);
      runAction(blink);
    }

  } else {
    PowerUp::tick();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Shield::activate() {

  if (_is_active) {
  return; }

  // if a shield already exists on the player,
  if (_ship->getShield()) {
    // reset the existing shield
    _ship->getShield()->reset();
    // deactivate self
    deactivate();
    removeFromParentAndCleanup(true);
  }
  // else if shield doesn't exist on the player
  else {
    PowerUp::activate();

    // set the shield duration
    _shield_time_left = SHIELD_DURATION;
    setScale(0);

    // generate & draw a bigger cyan hexagon
    s_vec<c::Vec2> vertices;
    getRegularPolygonVertices(vertices, 6, PLAYER_RADIUS * 2.5);
    drawPolygon(&vertices[0], 6,
        c::ccc4f(0, 0, 0, 0), 4, c::ccc4f(0, 0.96862, 1, 1));

    // animate the activation & life of the shield
    runAction(
        c::EaseBounceOut::create(c::ScaleTo::create(0.25, 1)));
    runAction(
        c::RepeatForever::create(
          c::Sequence::create(
            c::EaseSineOut::create(
              c::ScaleTo::create(0.25, 1.15)),
            c::EaseSineOut::create(
              c::ScaleTo::create(0.25, 1)),
            CC_NIL)));

    // inform the player that it now has a shield around it
    _ship->setShield(this);
  }

  cx::sfxPlay("shield");
}

//////////////////////////////////////////////////////////////////////////////
//
void Shield::deactivate() {

  PowerUp::deactivate();

  if (_is_active) {
    // inform the player that it no longer has a shield around it
    _ship->setShield(CC_NIL);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Shield::reset() {
  // reset the shield duration
  _shield_time_left = SHIELD_DURATION;
  // stop any blinking action & show the shield if it was hidden due to the blink
  stopActionByTag(SHIELD_BLINK_TAG);
  setVisible(true);
}


NS_END



