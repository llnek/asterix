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

#include "support/XConfig.h"
#include "support/CCSX.h"
#include "XHUDLayer.h"

NS_BEGIN(fusilli)


//////////////////////////////////////////////////////////////////////////////
//
XHUDLayer::~XHUDLayer() {
}

//////////////////////////////////////////////////////////////////////////////
//
XHUDLayer::XHUDLayer()
  : scoreLabel(nullptr)
  , lives( nullptr)
  , score(0)
  , replayBtn(nullptr) {
}

//////////////////////////////////////////////////////////////////////////////
//
void XHUDLayer::RemoveIcon(cc::Node* icon) {
  RemoveItem(icon);
}

//////////////////////////////////////////////////////////////////////////////
//
void XHUDLayer::AddIcon(cc::Node* icon, int* z, int* idx) {
  AddAtlasItem(HudAtlas(), icon, z, idx);
}

//////////////////////////////////////////////////////////////////////////////
//
XLayer* XHUDLayer::Realize() {
  InitAtlases();
  InitIcons();
  InitLabels();
  InitCtrlBtns();
  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
void XHUDLayer::InitAtlases() {}

//////////////////////////////////////////////////////////////////////////////
//
void XHUDLayer::InitIcons() {}

//////////////////////////////////////////////////////////////////////////////
//
void XHUDLayer::InitLabels() {}

//////////////////////////////////////////////////////////////////////////////
//
void XHUDLayer::InitCtrlBtns() {}

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
void XHUDLayer::AddMenuIcon(cc::MenuItem* b,
    const cc::Vec2& where) {

  auto tile= CstVal<cc::Integer>("TILE")->getValue();
  auto hh = CCSX::GetScaledHeight(b) * 0.5;
  auto hw = CCSX::GetScaledWidth(b) * 0.5;
  auto cfg = XConfig::GetInstance();
  auto menu= cc::Menu::create();
  auto wz= CCSX::VisBox();
  float x, y;

  menu->addChild(b);

  if (where.y == CCSX::AnchorB().y) {
    y = wz.bottom + tile  + hh;
  } else {
    y = wz.top - tile - hh;
  }
  menu->setPosition(wz.right - tile - hw, y);
  AddItem(menu);
}

//////////////////////////////////////////////////////////////////////////////
//
void XHUDLayer::AddReplayIcon(cc::MenuItem* c,
    const cc::Vec2& where) {

  auto tile= CstVal<cc::Integer>("TILE")->getValue();
  auto hh = CCSX::GetScaledHeight(c) * 0.5;
  auto hw = CCSX::GetScaledWidth(c) * 0.5;
  auto cfg = XConfig::GetInstance();
  auto menu= cc::Menu::create();
  auto wz= CCSX::VisBox();
  float x, y;

  menu->addChild(c);

  if (where.y == CCSX::AnchorB().y) {
    y = wz.bottom + tile  + hh;
  } else {
    y = wz.top - tile  - hh;
  }
  menu->setPosition(wz.left + tile + hw, y);
  replayBtn=menu;
  AddItem(menu);
}









NS_END(fusilli)

