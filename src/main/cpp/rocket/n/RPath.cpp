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

#include "x2d/GameScene.h"
#include "core/CCSX.h"
#include "RPath.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(rocket)

//////////////////////////////////////////////////////////////////////////////
//
bool RPath::init() {

  if (!c::DrawNode::init()) {
  return false; }

  auto wz=cx::visSize();
  dash = 10;
  dashSpace = 10;
  lineType = LINE_NONE;
  energyHeight = CC_ZH(wz) * 0.8;
  energyLineX = CC_ZW(wz) * 0.96;

  this->scheduleUpdate();
  this->reset();

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//
void RPath::reset() {
  energyDecrement = 0.005;
  energy = 1.0;
}

//////////////////////////////////////////////////////////////////////////////
//
void RPath::update(float dt) {
  auto c4= c::Color4F::WHITE;
  auto wb=cx::visBox();

  energy -= dt * energyDecrement;
  if (energy < 0) {
    energy = 0;
  }

  clear();

  switch (lineType) {

    case LINE_NONE:
    break;

    case LINE_TEMP:
      drawLine(tip, pivot, c::Color4F(1.0, 1.0, 1.0, 1.0));
      drawDot(pivot, 5, c4);
    break;

    case LINE_DASHED:
      auto segments = lineLength / (dash + dashSpace);
      auto t = 0.0f;
      float x_;
      float y_;
      drawDot(pivot, 5, c4);
      for (auto i = 0; i < segments + 1; ++i) {
        x_ = pivot.x + t * (tip.x - pivot.x);
        y_ = pivot.y + t * (tip.y - pivot.y);
        drawDot(c::Vec2(x_, y_), 5, c4);
        t += (float) 1 / segments;
      }
    break;
  }

  //draw energy bar
  drawLine(CCT_PT(energyLineX, wb.top * 0.1),
      CCT_PT(energyLineX, wb.top * 0.9),
      c::Color4F(0.0, 0.0, 0.0, 1.0));

  drawLine(CCT_PT(energyLineX, wb.top * 0.1),
      CCT_PT(energyLineX, wb.top * 0.1 + energy * energyHeight),
      c::Color4F(1.0, 0.5, 0.0, 1.0));
}

//////////////////////////////////////////////////////////////////////////////
//
void RPath::setEnergyDecrement(float value) {
  energyDecrement += value;
}


NS_END



