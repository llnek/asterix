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

#include "core/XConfig.h"
#include "2d/CCLabel.h"
#include "core/CCSX.h"
#include "x2d/XLives.h"
#include "x2d/MainGame.h"
#include "HUD.h"
NS_ALIAS(cx, fusii::ccsx)
NS_ALIAS(f, fusii)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////////
//
HUDLayer::~HUDLayer() {
}

//////////////////////////////////////////////////////////////////////////////
//
HUDLayer::HUDLayer() {
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::ShowMenu(c::Ref* r) {
  MGML()->SendMsg("/hud/showmenu", nullptr);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::OnReplay(c::Ref* r) {
  MGML()->SendMsg("/hud/replay", nullptr);
}

//////////////////////////////////////////////////////////////////////////////
//
f::XLayer* HUDLayer::Realize() {

  auto color=  c::Color3B::WHITE;//cx::White();
  auto scale=1;

  RegoAtlas("game-pics");
  InitLabels();
  InitIcons();

  auto r= cx::CreateMenuBtn("icon_replay.png");
  r->setTarget(this,
      CC_MENU_SELECTOR(HUDLayer::OnReplay));
  r->setColor(color);
  //r->setScale(scale);
  AddReplayIcon(r, cx::AnchorB());

  auto b= cx::CreateMenuBtn("icon_menu.png");
  b->setTarget(this,
      CC_MENU_SELECTOR(HUDLayer::ShowMenu));
  b->setColor(color);
  //b->setScale(scale);
  AddMenuIcon(b, cx::AnchorB());

  return this;
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::InitLabels() {
  auto soff = f::CstVal<c::Integer>("S_OFF")->getValue();
  auto tile = f::CstVal<c::Integer>("TILE")->getValue();
  auto wb = cx::VisBox();
  auto fp= XCFGS()->GetFont("font.SmallTypeWriting");

  scoreLabel = cx::CreateBmfLabel(0,0,fp, "0");
  scoreLabel->setAnchorPoint(cx::AnchorBR());
  scoreLabel->setScale(XCFGS()->GetScale());
  scoreLabel->setPosition(wb.right - tile - soff,
    wb.top - tile - soff - cx::GetScaledHeight(scoreLabel));

  this->addChild(scoreLabel, lastZix, ++lastTag);
}

//////////////////////////////////////////////////////////////////////////////
//
void HUDLayer::InitIcons() {
  auto soff = f::CstVal<c::Integer>("S_OFF")->getValue();
  auto tile = f::CstVal<c::Integer>("TILE")->getValue();
  auto wb = cx::VisBox();

  lives = f::XLives::Create( this, "health.png", 3, tile + soff,
    wb.top - tile - soff);
}




NS_END(invaders)



