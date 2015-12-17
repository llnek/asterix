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
owner<Config*> Config::reify() {
  auto c =  mc_new(Config);
  c->initAssets();
  c->initCsts();
  return c;
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initCsts() {
  addCst("PLAYER_THINK_TIME", CC_INT(7));
  addCst("SCENE_DELAY", CC_FLOAT(0.5f));
  addCst("CV_Z", CC_INT(0));
  addCst("showFPS", CC_BOOL(false));

  addColor("default", CC_STR("#5e3178"));
  addColor("x", CC_STR("#f6b17f"));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initAssets() {

  addAtlas("lang-pics", CC_STR("l10n/en/images.plist"));
  addAtlas("game-pics", CC_STR("pics/images.plist"));

  addImage("gui.edit.orange", CC_STR("pics/orange_edit.png"));
  addImage("game.bg", CC_STR("pics/bg.png"));
  addImage("game-pics", CC_STR("pics/images.png"));
  addImage("lang-pics", CC_STR("l10n/en/images.png"));
  addImage("gui.mmenu.menu.bg", CC_STR("pics/bg.png"));

  addEffect("game_end", CC_STR("sfx/MineExplosion.mp3"));
  addEffect("x_pick", CC_STR("sfx/ElevatorBeep.mp3"));
  addEffect("o_pick", CC_STR("sfx/MineBeep.mp3"));
  addEffect("game_quit", CC_STR("sfx/Death.mp3"));

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
void Config::handleResolution(const c::Size &rs) {
  //for default font, we use 48pt
  scale = 52.0f/256.0f * rs.width /320.0f;
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
const sstr Config::getWSUrl() {
  return "";
}

//////////////////////////////////////////////////////////////////////////////
//
const sstr getGameId() {
  return "";
}

//////////////////////////////////////////////////////////////////////////////
//
const sstr getRoomId() {
  return "";
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::setGameId(const sstr &s) {

}

//////////////////////////////////////////////////////////////////////////////
//
void Config::setRoomId(const sstr &s) {

}

//////////////////////////////////////////////////////////////////////////////
//
c::Scene* Config::prelude() {
  return Splash::reify();
}


NS_END(tttoe)

