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

#include "Splash.h"
#include "Config.h"
NS_BEGIN(tttoe)

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
Config::Config() {
  scale = 1.0f;
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::InitCsts() {

  AddCst("PLAYER_THINK_TIME", CC_INT(7));
  AddCst("CV_Z", CC_INT(0));
  AddCst("showFPS", CC_BOOL(false));
}

//////////////////////////////////////////////////////////////////////////
//
void Config::InitAssets() {

  AddAtlas("lang-pics", CC_STR("l10n/en/images.plist"));
  AddAtlas("game-pics", CC_STR("pics/images.plist"));

  AddImage("gui.edit.orange", CC_STR("pics/orange_edit.png"));
  AddImage("game.bg", CC_STR("pics/bg.png"));
  AddImage("gui.mmenu.menu.bg", CC_STR("pics/bg.png"));

  AddEffect("game_end", CC_STR("sfx/MineExplosion.mpg"));
  AddEffect("x_pick", CC_STR("sfx/ElevatorBeep.mpg"));
  AddEffect("o_pick", CC_STR("sfx/MineBeep.mpg"));
  AddEffect("game_quit", CC_STR("sfx/Death.mpg"));

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
void Config::HandleResolution(const c::Size& rs) {
  //for default font, we use 48pt
  scale = 52/256 * rs.width /320;
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
const stdstr Config::GetWSUrl() {
  return "";
}

//////////////////////////////////////////////////////////////////////////////
//
const stdstr GetGameId() {
  return "";
}

//////////////////////////////////////////////////////////////////////////////
//
const stdstr GetRoomId() {
  return "";
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::SetGameId(const stdstr& s) {

}

//////////////////////////////////////////////////////////////////////////////
//
void Config::SetRoomId(const stdstr& s) {

}

//////////////////////////////////////////////////////////////////////////////
//
c::Scene* Config::StartWith() {
  return Splash::Reify();
}

NS_END(tttoe)

