// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2015, Ken Leung. All rights reserved.

#include "core/CCSX.h"
#include "XLayer.h"
#include "XLives.h"
NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(fusii)

//////////////////////////////////////////////////////////////////////////////
//
void XLives::reduce(int x) {
  while (x > 0) {
    if (icons.size() > 0) {
      auto it= icons.back();
      it->removeFromParent();
      icons.pop_back();
    }
    --x;
    --curLives;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void XLives::reset() {
  F__LOOP(it, icons) { auto n= *it; n->removeFromParent(); }
  icons.clear();
  curLives = totalLives;
}

//////////////////////////////////////////////////////////////////////////////
//
void XLives::resurrect() {
  reset();
  drawLives();
}

//////////////////////////////////////////////////////////////////////////////
//
void XLives::drawLives() {
  float y;
  float x;

  for (int n = 0; n < curLives; ++n) {
    auto v= cx::reifySprite(frameId);
    v->setScale(scale);
    if (n==0) {
      lifeSize = v->getContentSize();
      y= refPt.y - HHZ(lifeSize);
      x= refPt.x + HWZ(lifeSize);
    }
    v->setPosition(x,y);
    addChild(v);
    icons.push_back(v);
    if (this->dir > 0) {
      x += lifeSize.width * 1.2f;
    } else {
      x -= lifeSize.width * 1.2f;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void XLives::decorate(const sstr& frame, int lives,
    float x, float y, float scale, int d) {

  totalLives = lives;
  frameId = frame;
  curLives = 0;
  dir = d;
  this->scale=scale;

  refPt= c::Vec2(x,y);
  reset();
  drawLives();
}

//////////////////////////////////////////////////////////////////////////////
//
XLives::~XLives() {
}

//////////////////////////////////////////////////////////////////////////////
//
XLives::XLives() {
  totalLives = 0;
  curLives = 0;
  dir = 1;
  scale=1.0f;
}


NS_END(fusii)

