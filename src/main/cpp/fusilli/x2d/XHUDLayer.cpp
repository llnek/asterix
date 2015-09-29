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

#include "core/Primitives.h"
#include "core/XConfig.h"
#include "core/CCSX.h"
#include "XHUDLayer.h"
#include "XLives.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(fusii)


//////////////////////////////////////////////////////////////////////////////
//
XHUDLayer::~XHUDLayer() {
}

//////////////////////////////////////////////////////////////////////////////
//
XHUDLayer::XHUDLayer() {
  SNPTR(scoreLabel)
  SNPTR(lives)
  score = 0;
  SNPTR(replayBtn)
}

//////////////////////////////////////////////////////////////////////////////
//
void XHUDLayer::AddIcon(not_null<XLive*> icon) {
  AddItem((c::Node*) icon.get());
}

//////////////////////////////////////////////////////////////////////////////
//
void XHUDLayer::ResetAsNew() {
  Reset();
}

//////////////////////////////////////////////////////////////////////////////
//
void XHUDLayer::Reset() {
  DisableReplay();
  score= 0;
  if (NNP(lives)) {
    lives->Resurrect();
  }
}

//////////////////////////////////////////////////////////////////////////////
//
bool XHUDLayer::ReduceLives(int x) {
  lives->Reduce(x);
  return lives->IsDead();
}

//////////////////////////////////////////////////////////////////////////////
//
void XHUDLayer::UpdateScore(int num) {
  score += num;
  scoreLabel->setString(s::to_string(score));
}

//////////////////////////////////////////////////////////////////////////////
//
void XHUDLayer::DisableReplay() {
  replayBtn->setVisible(false);
}

//////////////////////////////////////////////////////////////////////////////
//
void XHUDLayer::EnableReplay() {
  replayBtn->setVisible(true);
}

//////////////////////////////////////////////////////////////////////////////
//
void XHUDLayer::AddMenuIcon(not_null<c::MenuItem*> b,
    const c::Vec2& where) {

  auto tile= CstVal<c::Integer>("TILE")->getValue();
  auto hh = cx::GetScaledHeight(b) * 0.5;
  auto hw = cx::GetScaledWidth(b) * 0.5;
  auto menu= c::Menu::create();
  auto wz= cx::VisBox();
  float x, y;

  menu->addChild(b);

  if (where.y == cx::AnchorB().y) {
    y = wz.bottom + tile  + hh;
  } else {
    y = wz.top - tile - hh;
  }

  menu->setPosition(wz.right - tile - hw, y);
  AddItem(menu);
}

//////////////////////////////////////////////////////////////////////////////
//
void XHUDLayer::AddReplayIcon(not_null<c::MenuItem*> c,
    const c::Vec2& where) {

  auto tile= CstVal<c::Integer>("TILE")->getValue();
  auto hh = cx::GetScaledHeight(c) * 0.5;
  auto hw = cx::GetScaledWidth(c) * 0.5;
  auto menu= c::Menu::create();
  auto wz= cx::VisBox();
  float x, y;

  menu->addChild(c);

  if (where.y == cx::AnchorB().y) {
    y = wz.bottom + tile  + hh;
  } else {
    y = wz.top - tile  - hh;
  }

  menu->setPosition(wz.left + tile + hw, y);
  replayBtn=menu;
  AddItem(menu);
}









NS_END(fusii)

