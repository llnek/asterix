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

#include "Config.h"
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////////
//
const s::string Config::GetAppKey() {
  return "d39bf198-518a-4de7-88a0-5e28c88571b0";
}

//////////////////////////////////////////////////////////////////////////////
//
const s::string Config::AppId() {
  return "invaders";
}

//////////////////////////////////////////////////////////////////////////////
//
const s::string Config::GetColor() {
  return "red";
}

//////////////////////////////////////////////////////////////////////////////
//
ResolutionPolicy Config::GetResolution() {
  return ResolutionPolicy::FIXED_HEIGHT;
}

//////////////////////////////////////////////////////////////////////////////
//
cc::Scene* Config::StartWith() {
  return Splash::Create();
}

//////////////////////////////////////////////////////////////////////////////
//
Config::Config()
  : scale(1) {

  InitAssets();
  InitCsts();
  InitPools();

}

//////////////////////////////////////////////////////////////////////////////
//
const cc::Size Config::GetGameSize() {
  return cc::Size(320, 480);
}

//////////////////////////////////////////////////////////////////////////////
//
float Config::GetScale() {
  return scale;
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::HandleResolution(const cc::Size& rs) {
  //for default font, we use 48pt
  scale = 52/256 * rs.width /320;
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::RunOnce() {
  auto c= cc::SpriteFrameCache::getInstance();
  c->addSpriteFrames( GetAtlas("game-pics"));
  c->addSpriteFrames( GetAtlas("lang-pics"));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::InitAssets() {

  auto d = GetFragment(ATLASES);

  d->setObject("res/l10n/en/images.png", "lang-pics");
  d->setObject("res/pics/images.png", "game-pics");

  d= GetFragment(IMAGES);
  d->setObject("res/pics/bg.png", "gui.mmenus.menu.bg");
  d->setObject("res/pics/bg.png", "game.bg");

  d= GetFragment(SOUNDS);
  d->setObject("res/sfx/MineExplosion", "game_end");
  d->setObject("res/sfx/Death", "game_quit");
  d->setObject("res/sfx/missile", "ship-missile");
  d->setObject("res/sfx/march", "bugs-march");
  d->setObject("res/sfx/explode", "xxx-explode");

  d= GetFragment(FONTS);
  d->setObject("res/font/en/SmallTypeWriting", "font.SmallTypeWriting");
  d->setObject("res/font/en/AutoMission", "font.AutoMission");
  d->setObject("res/font/en/Subito", "font.Subito");
  d->setObject("res/font/en/CoffeeBuzzed", "font.CoffeeBuzzed");

}

//////////////////////////////////////////////////////////////////////////////
//
void Config::InitCsts() {
  auto d = GetFragment(CSTS);

  d->setObject("live-missiles", "P_LMS");
  d->setObject("missiles", "P_MS");
  d->setObject("bombs", "P_BS");
  d->setObject("explosions", "P_ES");
  d->setObject("live-bombs", "P_LBS");

  d->setObject(42, "CELLS");
  d->setObject(8, "TILE");
  d->setObject(6, "COLS");
  d->setObject(7, "ROWS");

  d->setObject(2, "LEFT");
  d->setObject(6, "TOP");
  d->setObject(4, "OFF_X");
  d->setObject(2, "OFF_Y");
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::InitPools() {
  auto d = GetFragment(POOLS);

  d->setObject(new XPool(), "missiles");
  d->setObject(new XPool(), "bombs");
  d->setObject(new XPool(), "explosions");

}

//////////////////////////////////////////////////////////////////////////
//
const s::string Config::GetWSUrl() {
  return "";
}

//////////////////////////////////////////////////////////////////////////
//
const s::string GetGameId() {

}

//////////////////////////////////////////////////////////////////////////
//
const s::string GetRoomId() {

}



NS_END(invaders)


