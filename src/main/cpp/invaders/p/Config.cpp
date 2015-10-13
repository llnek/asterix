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
}

//////////////////////////////////////////////////////////////////////////////
//
owner<f::XConfig*> Config::Create() {
  Config* c = new Config();
  c->InitAssets();
  c->InitCsts();
  return c;
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
  scale = 0.15;//52/256 * rs.width /320;
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::RunOnce() {
  auto c= c::SpriteFrameCache::getInstance();
  auto fp= GetAtlas("game-pics");
  c->addSpriteFramesWithFile( fp);
  CCLOG("loaded sprites sheets %s", fp.c_str());
  fp=  GetAtlas("lang-pics");
  c->addSpriteFramesWithFile(fp);
  CCLOG("loaded sprites sheets %s", fp.c_str());
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::InitAssets() {

  AddAtlas("lang-pics", c::String::create("l10n/en/images.plist"));
  AddAtlas("game-pics", c::String::create("pics/images.plist"));

  AddImage("mmenus.bg", c::String::create("pics/bg.png"));
  AddImage("game.bg", c::String::create("pics/bg.png"));
  AddImage("lang-pics", c::String::create("l10n/en/images.png"));
  AddImage("game-pics", c::String::create("pics/images.png"));

  AddEffect("game_end", c::String::create("sfx/MineExplosion.mp3"));
  AddEffect("game_quit", c::String::create("sfx/Death.mp3"));
  AddEffect("ship-missile", c::String::create("sfx/missile.mp3"));
  AddEffect("bugs-march", c::String::create("sfx/march.mp3"));
  AddEffect("xxx-explode", c::String::create("sfx/explode.mp3"));

  AddFont("font.SmallTypeWriting", c::String::create("fon/en/SmallTypeWriting.fnt"));
  AddFont("font.AutoMission", c::String::create("fon/en/AutoMission.fnt"));
  AddFont("font.Subito", c::String::create("fon/en/Subito.fnt"));
  AddFont("font.CoffeeBuzzed", c::String::create("fon/en/CoffeeBuzzed.fnt"));

  AddFont("font.TinyBoxBB", c::String::create("fon/en/TinyBoxBlackBitA8.fnt"));
  AddFont("font.OogieBoogie", c::String::create("fon/en/OogieBoogie.fnt"));
  AddFont("font.JellyBelly", c::String::create("fon/en/JellyBelly.fnt"));
  AddFont("font.AgentOrange", c::String::create("fon/en/AgentOrange.fnt"));
  AddFont("font.Hiruko", c::String::create("fon/en/Hiruko.fnt"));
  AddFont("font.OCR", c::String::create("fon/en/OCR.fnt"));

}

//////////////////////////////////////////////////////////////////////////////
//
void Config::InitCsts() {

  AddCst("showFPS", c::Bool::create(false));

  AddCst("CELLS", c::Integer::create(42));
  AddCst("COLS", c::Integer::create(6));
  AddCst("ROWS", c::Integer::create(7));

  AddCst("LEFT", c::Integer::create(2));
  AddCst("TOP", c::Integer::create(6));
  AddCst("OFF_X", c::Integer::create(4));
  AddCst("OFF_Y", c::Integer::create(2));

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


