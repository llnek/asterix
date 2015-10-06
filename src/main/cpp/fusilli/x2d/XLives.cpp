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
#include "XLive.h"
#include "XLives.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(fusii)


//////////////////////////////////////////////////////////////////////////////
//
void XLives::Reduce(int x) {
  while (x > 0) {
    if (icons.size() > 0) {
      auto it= icons.begin();
      hud->RemoveItem( *it );
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
    hud->RemoveItem(*it);
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
      lifeSize = c::Size(cx::GetScaledWidth(v),
                          cx::GetScaledHeight(v));
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
owner<XLives*> XLives::Create(not_null<XHUDLayer*> hud,
    const stdstr& frame,
    int lives,
    float x, float y, int dir) {

  auto vs = new XLives(lives, dir);

  vs->topLeft= c::Vec2(x,y);
  vs->frameId = frame;
  vs->hud= hud;
  vs->Reset();
  vs->DrawLives();

  return vs;
}

//////////////////////////////////////////////////////////////////////////////
//
XLives::~XLives() {
}

//////////////////////////////////////////////////////////////////////////////
//
XLives::XLives(int lives, int dir) {
  hud= nullptr;
  curLives = -1;
  totalLives = lives;
  this->dir = dir;
}




NS_END(fusii)
