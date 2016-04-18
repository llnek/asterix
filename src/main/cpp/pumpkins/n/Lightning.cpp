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
#include "Lightning.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(pumpkins)

//////////////////////////////////////////////////////////////////////////////
owner<Lightning*> Lightning::create(
    const CCT_PT &from, const CCT_PT &to,
    c::Color4F color, bool is_animated) {
  auto z = mc_new(Lightning);
  z->inix(from, to, color, is_animated);
  z->autorelease();
  return z;
}

//////////////////////////////////////////////////////////////////////////////
//
bool Lightning::inix(const CCT_PT &from,
    cont CCT_PT &to, c::Color4F color, bool is_animated) {

  if ( ! c::DrawNode::init()) {
  return false;
  }

  _color = color;
  generateKeyPoints(from , to);
  if (!is_animated) {
    drawSegments();
  } else {
    schedule(schedule_selector(Lightning::drawNextSegment));
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void Lightning::generateKeyPoints(const CCT_PT &from, const CCT_PT &to) {

  // how many key points do we need?
  auto dist= c::ccpDistance(from, to);
  _numKeyPoints = (int)(dist/ LIGHTNING_KEY_POINT_DIST);

  // calculate the difference between two key points
  auto next_point = CC_ZPT;
  auto delta = CCT_PT( (to.x - from.x) / _numKeyPoints,
                       (to.y - from.y) / _numKeyPoints);
  for (auto i = 0; i < _numKeyPoints; ++i) {
    // add the delta
    next_point = c::ccpAdd(from, c::ccpMult(delta, i));
    // randomise the delta
    next_point.x += LIGHTNING_KEY_POINT_DIST * CCRANDOM_MINUS1_1();
    next_point.y += LIGHTNING_KEY_POINT_DIST * CCRANDOM_MINUS1_1();
    // save the key point
    _keyPoints.push_back(next_point);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Lightning::drawSegments() {
  // draw all segments at once
  for (auto i = 0; i < _numKeyPoints - 1; ++i) {
    drawSegment(_keyPoints[i], _keyPoints[i+1], 6, _color);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Lightning::drawNextSegment(float dt) {
  // draw one segment at a time
  if (++_lastKeyPoint >= _numKeyPoints - 2) {
    unschedule(schedule_selector(Lightning::drawNextSegment));
  }

  drawSegment(_keyPoints[_lastKeyPoint],
      _keyPoints[_lastKeyPoint+1], 6, _color);

}



NS_END



