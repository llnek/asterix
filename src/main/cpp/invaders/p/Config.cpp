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
const stdstr Config::appKey() {
  return "d39bf198-518a-4de7-88a0-5e28c88571b0";
}

//////////////////////////////////////////////////////////////////////////////
//
const stdstr Config::appId() {
  return "invaders";
}

//////////////////////////////////////////////////////////////////////////////
//
const stdstr Config::color() {
  return "red";
}

//////////////////////////////////////////////////////////////////////////////
//
ResolutionPolicy Config::policy() {
  return ResolutionPolicy::FIXED_HEIGHT;
}

//////////////////////////////////////////////////////////////////////////////
//
c::Scene* Config::startWith() {
  return Splash::reify();
}

//////////////////////////////////////////////////////////////////////////////
//
Config::Config() {
  scale = 1;
}

//////////////////////////////////////////////////////////////////////////////
//
owner<f::XConfig*> Config::reify() {
  auto c =  mc_new(Config);
  c->initAssets();
  c->initCsts();
  return c;
}

//////////////////////////////////////////////////////////////////////////////
//
const c::Size Config::gameSize() {
  return c::Size(320, 480);
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::handleResolution(const c::Size& rs) {
  //for default font, we use 48pt
  scale = 52/256.0f * rs.width / 320.0f;
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::runOnce() {
  auto c= c::SpriteFrameCache::getInstance();
  auto fp= getAtlas("game-pics");
  c->addSpriteFramesWithFile( fp);
  CCLOG("loaded sprite-sheet: %s", fp.c_str());
  fp= getAtlas("lang-pics");
  c->addSpriteFramesWithFile(fp);
  CCLOG("loaded sprite-sheet: %s", fp.c_str());
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initAssets() {

  addAtlas("lang-pics", CC_STR("l10n/en/images.plist"));
  addAtlas("game-pics", CC_STR("pics/images.plist"));

  addImage("mmenus.bg", CC_STR("pics/bg.png"));
  addImage("game.bg", CC_STR("pics/bg.png"));
  addImage("lang-pics", CC_STR("l10n/en/images.png"));
  addImage("game-pics", CC_STR("pics/images.png"));

  addEffect("game_end", CC_STR("sfx/MineExplosion.mp3"));
  addEffect("game_quit", CC_STR("sfx/Death.mp3"));
  addEffect("ship-missile", CC_STR("sfx/missile.mp3"));
  addEffect("bugs-march", CC_STR("sfx/march.mp3"));
  addEffect("xxx-explode", CC_STR("sfx/explode.mp3"));

  addFont("font.SmallTypeWriting", CC_STR("fon/en/SmallTypeWriting.fnt"));
  addFont("font.AutoMission", CC_STR("fon/en/AutoMission.fnt"));
  addFont("font.Subito", CC_STR("fon/en/Subito.fnt"));
  addFont("font.CoffeeBuzzed", CC_STR("fon/en/CoffeeBuzzed.fnt"));

  addFont("font.TinyBoxBB", CC_STR("fon/en/TinyBoxBlackBitA8.fnt"));
  addFont("font.OogieBoogie", CC_STR("fon/en/OogieBoogie.fnt"));
  addFont("font.JellyBelly", CC_STR("fon/en/JellyBelly.fnt"));
  addFont("font.AgentOrange", CC_STR("fon/en/AgentOrange.fnt"));
  addFont("font.Hiruko", CC_STR("fon/en/Hiruko.fnt"));
  addFont("font.OCR", CC_STR("fon/en/OCR.fnt"));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initCsts() {

  addCst("showFPS", CC_BOOL(false));
  addCst("CELLS",  CC_INT(42));
  addCst("COLS",  CC_INT(6));
  addCst("ROWS",  CC_INT(7));
  addCst("LEFT", CC_INT(2));
  addCst("TOP", CC_INT(6));
  addCst("OFF_X", CC_INT(4));
  addCst("OFF_Y", CC_INT(2));
}

//////////////////////////////////////////////////////////////////////////
//
const stdstr Config::getWSUrl() {
  return "";
}

//////////////////////////////////////////////////////////////////////////
//
const stdstr Config::getGameId() {
  return "";
}

//////////////////////////////////////////////////////////////////////////
//
const stdstr Config::getRoomId() {
  return "";
}



NS_END(invaders)


