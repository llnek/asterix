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
c::Scene* Config::StartWith() {
  return Splash::Reify();
}

//////////////////////////////////////////////////////////////////////////////
//
Config::Config() {
  scale = 1;
}

//////////////////////////////////////////////////////////////////////////////
//
owner<f::XConfig*> Config::Reify() {
  auto c =  mc_new(Config);
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
  CCLOG("loaded sprite-sheet: %s", fp.c_str());
  fp=  GetAtlas("lang-pics");
  c->addSpriteFramesWithFile(fp);
  CCLOG("loaded sprite-sheet: %s", fp.c_str());
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::InitAssets() {

  AddAtlas("lang-pics", CC_STR("l10n/en/images.plist"));
  AddAtlas("game-pics", CC_STR("pics/images.plist"));

  AddImage("mmenus.bg", CC_STR("pics/bg.png"));
  AddImage("game.bg", CC_STR("pics/bg.png"));
  AddImage("lang-pics", CC_STR("l10n/en/images.png"));
  AddImage("game-pics", CC_STR("pics/images.png"));

  AddEffect("game_end", CC_STR("sfx/MineExplosion.mp3"));
  AddEffect("game_quit", CC_STR("sfx/Death.mp3"));
  AddEffect("ship-missile", CC_STR("sfx/missile.mp3"));
  AddEffect("bugs-march", CC_STR("sfx/march.mp3"));
  AddEffect("xxx-explode", CC_STR("sfx/explode.mp3"));

  AddFont("font.SmallTypeWriting", CC_STR("fon/en/SmallTypeWriting.fnt"));
  AddFont("font.AutoMission", CC_STR("fon/en/AutoMission.fnt"));
  AddFont("font.Subito", CC_STR("fon/en/Subito.fnt"));
  AddFont("font.CoffeeBuzzed", CC_STR("fon/en/CoffeeBuzzed.fnt"));

  AddFont("font.TinyBoxBB", CC_STR("fon/en/TinyBoxBlackBitA8.fnt"));
  AddFont("font.OogieBoogie", CC_STR("fon/en/OogieBoogie.fnt"));
  AddFont("font.JellyBelly", CC_STR("fon/en/JellyBelly.fnt"));
  AddFont("font.AgentOrange", CC_STR("fon/en/AgentOrange.fnt"));
  AddFont("font.Hiruko", CC_STR("fon/en/Hiruko.fnt"));
  AddFont("font.OCR", CC_STR("fon/en/OCR.fnt"));

}

//////////////////////////////////////////////////////////////////////////////
//
void Config::InitCsts() {

  AddCst("showFPS", CC_BOOL(false));
  AddCst("CELLS",  CC_INT(42));
  AddCst("COLS",  CC_INT(6));
  AddCst("ROWS",  CC_INT(7));
  AddCst("LEFT", CC_INT(2));
  AddCst("TOP", CC_INT(6));
  AddCst("OFF_X", CC_INT(4));
  AddCst("OFF_Y", CC_INT(2));

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


