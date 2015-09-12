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

#include "XLives.h"
NS_USING(cocos2d)
NS_USING(std)
NS_BEGIN(fusilli)


//////////////////////////////////////////////////////////////////////////////
//
void XLives::Reduce(int x) {
  while (x > 0) {
    if (icons.size() > 0) {
      auto it= icons.begin();
      hud->RemoveIcon( *it );
      icons.erase(it);
    }
    --x;
    --curLives;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
bool XLives::IsDead() {
  return curLives < 0;
}

//////////////////////////////////////////////////////////////////////////////
//
void XLives::Reset() {
  for (auto it= icons.begin(); it != icons.end(); ++it) {
    hud->RemoveIcon(*it);
  }
  curLives = totalLives;
  icons.clear();
}

//////////////////////////////////////////////////////////////////////////////
//
void XLives::Resurrect() {
  Reset();
  DrawLives();
}

//////////////////////////////////////////////////////////////////////////////
//
void XLives::DrawLives() {
  auto y= topLeft.y - lifeSize.height * 0.5;
  auto x= topLeft.x + lifeSize.width * 0.5;
  auto gap=2;

  for (int n = 0; n < curLives; ++n) {
    auto v= XLive::Create(frameId);
    v->setPosition(x,y);
    hud->AddIcon(v);
    icons.push_back(v);
    if (this->dir> 0) {
      x += lifeSize.width + gap;
    } else {
      x -= lifeSize.width - gap;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void XLives::Create() {
  XLive* dummy = XLive::Create(frameId);
  //autorelease, so is ok
  lifeSize = Size(CCSX::GetScaledWidth(dummy),
                  CCSX::GetScaledHeight(dummy));
  DrawLives();
}

//////////////////////////////////////////////////////////////////////////////
//
XLives::XLives(XHUDLayer* hud, const string& frame, float x, float y, int dir)
: XLives() {
  topLeft= Vec2(x,y);
  frameId = frame;
  this->hud= hud;
  Reset();
  this->dir=dir;
}

//////////////////////////////////////////////////////////////////////////////
//
XLives::~XLives() {
}

//////////////////////////////////////////////////////////////////////////////
//
XLives::XLives()
: hud(nullptr),
  curLives(-1),
  totalLives(0),
  dir(1) {
}


NS_END(fusilli)
