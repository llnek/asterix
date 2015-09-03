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

#include "HUD.h"
NS_FI_BEGIN

void HUDLayer::InitAtlases() {
  RegoAtlas("game-pics");
}

void HUDLayer::InitLabels() {
  auto wb = CCSX::VisBox();
  m_scoreLabel = CreateBmfLabel("font.SmallTypeWriting", "0");
  m_scoreLabel->setAnchor(Anchor::BottomRight);
  m_scoreLabel->setScale(xcfg.game.scale);
  m_scoreLabel->setPosition(wb.right - csts.TILE - csts.S_OFF,
    wb.top - csts.TILE - csts.S_OFF - CCSX::GetScaledHeight(m_scoreLabel));
  addChild(m_scoreLabel, m_lastZix, ++m_lastTag);
}

void HUDLayer::InitIcons() {
  auto wb = CCSX::VisBox();
  m_lives = new scenes.XHUDLives( this, csts.TILE + csts.S_OFF,
    wb.top - csts.TILE - csts.S_OFF,
    "health.png",
    3);
  m_lives->Setup();
}

HUDLayer::HUDLayer() {
  auto color=  CCSX::WHITE;
  auto scale=1;

  auto r= CreateMenuBtn("#icon_replay.png",
      "#icon_replay.png",
      "#icon_replay.png",
      CC_CALLBACK_1(HUDLayer::OnReplay, this));
  r->setVisible(false);
  r->setColor(color);
  //r->setScale(scale);
  AddReplayIcon(r, Anchor::Bottom);

  auto b= CreateMenuBtn("#icon_menu.png",
      "#icon_menu.png",
      "#icon_menu.png",
      CC_CALLBACK_1(HUDLayer::ShowMenu, this));
  b->setColor(color);
  //b->setScale(scale);
  AddMenuIcon(b, Anchor::Bottom);
}







NS_FI_END

