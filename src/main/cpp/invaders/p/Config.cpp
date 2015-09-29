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

#include "core/XPool.h"
#include "Splash.h"
#include "Config.h"
NS_BEGIN(invaders)


//////////////////////////////////////////////////////////////////////////////
//
const stdstr Config::GetAppKey() {
  return "d39bf198-518a-4de7-88a0-5e28c88571b0";
}

//////////////////////////////////////////////////////////////////////////////
//
const stdstr Config::AppId() {
  return "invaders";
}

//////////////////////////////////////////////////////////////////////////////
//
const stdstr Config::GetColor() {
  return "red";
}

//////////////////////////////////////////////////////////////////////////////
//
ResolutionPolicy Config::GetPolicy() {
  return ResolutionPolicy::FIXED_HEIGHT;
}

//////////////////////////////////////////////////////////////////////////////
//
owner<c::Scene*> Config::StartWith() {
  return Splash::Create();
}

//////////////////////////////////////////////////////////////////////////////
//
Config::Config() {
  scale = 1;
  InitAssets();
  InitCsts();
  InitPools();
}

//////////////////////////////////////////////////////////////////////////////
//
const c::Size Config::GetGameSize() {
  return c::Size(320, 480);
}

//////////////////////////////////////////////////////////////////////////////
//
float Config::GetScale() {
  return scale;
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::HandleResolution(const c::Size& rs) {
  //for default font, we use 48pt
  scale = 52/256 * rs.width /320;
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::RunOnce() {
  auto c= c::SpriteFrameCache::getInstance();
  c->addSpriteFramesWithFile( GetAtlas("game-pics"));
  c->addSpriteFramesWithFile( GetAtlas("lang-pics"));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::InitAssets() {


  auto d = GetFragment(ATLASES);

  d->setObject(c::String::create("l10n/en/images.plist"), "lang-pics");
  d->setObject(c::String::create("pics/images.plist"), "game-pics");

  d= GetFragment(IMAGES);
  d->setObject(c::String::create("pics/bg.png"), "mmenus.bg");
  d->setObject(c::String::create("pics/bg.png"), "game.bg");

  d= GetFragment(SOUNDS);
  d->setObject(c::String::create("sfx/MineExplosion.mp3"), "game_end");
  d->setObject(c::String::create("sfx/Death.mp3"), "game_quit");
  d->setObject(c::String::create("sfx/missile.mp3"), "ship-missile");
  d->setObject(c::String::create("sfx/march.mp3"), "bugs-march");
  d->setObject(c::String::create("sfx/explode.mp3"), "xxx-explode");

  d= GetFragment(FONTS);
  d->setObject(c::String::create("fon/en/SmallTypeWriting.fnt"), "font.SmallTypeWriting");
  d->setObject(c::String::create("fon/en/AutoMission.fnt"), "font.AutoMission");
  d->setObject(c::String::create("fon/en/Subito.fnt"), "font.Subito");
  d->setObject(c::String::create("fon/en/CoffeeBuzzed.fnt"), "font.CoffeeBuzzed");

  d->setObject(c::String::create("fon/en/TinyBoxBlackBitA8.fnt"), "font.TinyBoxBB");
  d->setObject(c::String::create("fon/en/OogieBoogie.fnt"), "font.OogieBoogie");
  d->setObject(c::String::create("fon/en/JellyBelly.fnt"), "font.JellyBelly");
  d->setObject(c::String::create("fon/en/AgentOrange.fnt"), "font.AgentOrange");
  d->setObject(c::String::create("fon/en/Hiruko.fnt"), "font.Hiruko");
  d->setObject(c::String::create("fon/en/OCR.fnt"), "font.OCR");

}

//////////////////////////////////////////////////////////////////////////////
//
void Config::InitCsts() {

  auto d = GetFragment(CSTS);

  d->setObject(c::String::create("live-missiles"), "P_LMS");
  d->setObject(c::String::create("missiles"), "P_MS");
  d->setObject(c::String::create("bombs"), "P_BS");
  d->setObject(c::String::create("explosions"), "P_ES");
  d->setObject(c::String::create("live-bombs"), "P_LBS");

  d->setObject(c::Integer::create(42), "CELLS");
  d->setObject(c::Integer::create(8), "TILE");
  d->setObject(c::Integer::create(6), "COLS");
  d->setObject(c::Integer::create(7), "ROWS");

  d->setObject(c::Integer::create(2), "LEFT");
  d->setObject(c::Integer::create(6), "TOP");
  d->setObject(c::Integer::create(4), "OFF_X");
  d->setObject(c::Integer::create(2), "OFF_Y");
  d->setObject(c::Integer::create(4), "S_OFF");
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::InitPools() {
  c::Dictionary* d = GetFragment(POOLS);

  d->setObject(f::XPool::create(), "missiles");
  d->setObject(f::XPool::create(), "bombs");
  d->setObject(f::XPool::create(), "explosions");
}

//////////////////////////////////////////////////////////////////////////
//
const stdstr Config::GetWSUrl() {
  return "";
}

//////////////////////////////////////////////////////////////////////////
//
const stdstr GetGameId() {
  return "";
}

//////////////////////////////////////////////////////////////////////////
//
const stdstr GetRoomId() {
  return "";
}



NS_END(invaders)


