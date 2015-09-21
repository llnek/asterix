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

#include "2d/CCLabel.h"
#include "core/CCSX.h"
#include "HUD.h"
NS_ALIAS(cx, fusilli::ccsx)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////////
//
HUDLayer::~HUDLayer() {
}

//////////////////////////////////////////////////////////////////////////////
//
HUDLayer::HUDLayer()
  : m_scoreLabel(nullptr) {
}

//////////////////////////////////////////////////////////////////////////////
//
f::XLayer* HUDLayer::Realize() {
  auto color=  cx::White();
  auto scale=1;

  auto r= cx::CreateMenuBtn("#icon_replay.png");
  r->setTarget(this,
      CC_MENU_SELECTOR(HUDLayer::OnReplay));
  r->setVisible(false);
  r->setColor(color);
  //r->setScale(scale);
  AddReplayIcon(r, cx::AnchorB());

  auto b= cx::CreateMenuBtn("#icon_menu.png");
  b->setTarget(this,
      CC_MENU_SELECTOR(HUDLayer::ShowMenu));
  b->setColor(color);
  //b->setScale(scale);
  AddMenuIcon(b, cx::AnchorB());

  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::InitAtlases() {
  RegoAtlas("game-pics");
}

void HUDLayer::InitLabels() {
  auto soff = CstVal<c::Integer>("S_OFF")->getValue();
  auto tile = CstVal<c::Integer>("TILE")->getValue();
  auto cfg = f::XConfig::GetInstance();
  auto wb = cx::VisBox();
  m_scoreLabel = cx::CreateBmfLabel("font.SmallTypeWriting", "0");
  m_scoreLabel->setAnchorPoint(cx::AnchorBR());
  m_scoreLabel->setScale(cfg->GetScale());
  m_scoreLabel->setPosition(wb.right - tile - soff,
    wb.top - tile - soff - cx::GetScaledHeight(m_scoreLabel));
  addChild(m_scoreLabel, m_lastZix, ++m_lastTag);
}

void HUDLayer::InitIcons() {
  auto soff = CstVal<c::Integer>("S_OFF")->getValue();
  auto tile = CstVal<c::Integer>("TILE")->getValue();
  auto wb = cx::VisBox();
  lives = new f::XLives( this, "health.png", tile + soff,
    wb.top - tile - soff);
}




NS_END(invaders)



