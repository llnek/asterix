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
  auto gap=2;
  float y;
  float x;

  for (int n = 0; n < curLives; ++n) {
    auto v= XLive::Create(frameId);
    if (n==0) {
      lifeSize = cc::Size(CCSX::GetScaledWidth(v),
                          CCSX::GetScaledHeight(v));
      y= topLeft.y - lifeSize.height * 0.5;
      x= topLeft.x + lifeSize.width * 0.5;
    }
    v->setPosition(x,y);
    hud->AddIcon(v);
    icons.push_back(v);
    if (this->dir > 0) {
      x += lifeSize.width + gap;
    } else {
      x -= lifeSize.width - gap;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
XLives* XLives::Create(XHUDLayer* hud,
    const s::string& frame,
    float x, float y, int dir) {

  auto vs = new XLives();

  vs->topLeft= cc::Vec2(x,y);
  vs->frameId = frame;
  vs->hud= hud;
  vs->Reset();
  vs->dir=dir;
  vs->DrawLives();

  return vs;
}

//////////////////////////////////////////////////////////////////////////////
//
XLives::~XLives() {
}

//////////////////////////////////////////////////////////////////////////////
//
XLives::XLives()
  : hud(nullptr)
  , curLives(-1)
  , totalLives(0)
  , dir(1) {

}




NS_END(fusilli)
