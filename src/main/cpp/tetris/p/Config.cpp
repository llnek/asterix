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
#include "Splash.h"

NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////////
//
owner<Config*> Config::reify() {
  auto c = mc_new(Config);
  c->initAssets();
  c->initCsts();
  return c;
}

//////////////////////////////////////////////////////////////////////////////
//
c::Scene* Config::prelude() {
  return Splash::reify();
}

//////////////////////////////////////////////////////////////////////////
//
void Config::initCsts() {
  addCst("DROPSPEED", CC_FLOAT(1000.0f));
  addCst("BLOCK_COLORS", CC_INT(8));
  addCst("FIELD_W", CC_INT(12));
  addCst("THROTTLEWAIT", CC_INT(100));
  addCst("BTN_SIZE", CC_INT(32));

  addCst("FENCE", CC_INT(0));
  addCst("TILE", CC_INT(16));
    //CBOX: null,

  addColor("default", CC_STR("#f6b17f"));
  addColor("text", CC_STR("#ffffff"));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initAssets() {

  addAtlas("lang-pics", CC_STR("l10n/en/images.plist"));
  addAtlas("game-pics", CC_STR("pics/images.plist"));

  addImage("gui.mmenu.menu.bg", CC_STR("pics/bg.png"));
  addImage("game.bg", CC_STR("pics/bg.png"));

  addImage("lang-pics", CC_STR("l10n/en/images.png"));
  addImage("game-pics", CC_STR("pics/images.png"));

  addEffect("game_end", CC_STR("sfx/MineExplosion.mpg"));
  addEffect("game_quit", CC_STR("sfx/Death.mpg"));

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
  scale = 12.0f/72.0f * rs.width /480.0f;
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::runOnce() {
  auto c= c::SpriteFrameCache::getInstance();
  auto fp= getAtlas("game-pics");

  c->addSpriteFramesWithFile(fp);
  CCLOG("loaded sprite-sheet: %s", fp.c_str());

  fp = getAtlas("lang-pics");
  c->addSpriteFramesWithFile(fp);
  CCLOG("loaded sprite-sheet: %s", fp.c_str());
}



NS_END(tetris)

