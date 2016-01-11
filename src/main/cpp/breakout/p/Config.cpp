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

NS_BEGIN(breakout)

BEGIN_NS_UNAMED()
const s_arr<sstr,8> CANDIES {
  "red_candy.png", "amber_candy.png","white_candy.png","green_candy.png",
  "yellow_candy.png","blue_candy.png", "purple_plus_candy.png", "purple_minus_candy.png"
};

END_NS_UNAMED()
//////////////////////////////////////////////////////////////////////////////
//
owner<Config*> Config::reify() {
  auto c= mc_new(Config);
  c->initAssets();
  c->initCsts();
  c->initLevels();
  return c;
}

//////////////////////////////////////////////////////////////////////////////
//
const sstr Config::getCandy(int pos) {
  assert(pos >=0 && pos < CANDIES.size());
  return CANDIES[pos];
}

//////////////////////////////////////////////////////////////////////////////
//
c::Scene* Config::prelude() {
  return Splash::reify();
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initCsts() {
  game_id = "7d943e06-0849-4bf4-a16d-64a401f72a3e";
  app_id= "breakout";

  addCst("showFPS", CC_BOOL(false));
  addCst("GRID_W", CC_INT(40));
  addCst("GRID_H", CC_INT(60));

  addCst("PADDLE+OFF", CC_INT(4));
  addCst("LEFT+OFF", CC_INT(4));

  addColor("default", CC_STR("#f6b17f"));
  addColor("text", CC_STR("#ffffff"));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initAssets() {
  addAtlas("lang-pics", CC_STR("l10n/en/images.plist"));
  addAtlas("game-pics", CC_STR("pics/sprites.plist"));
  addAtlas("img-pics", CC_STR("pics/images.plist"));

  addImage("lang-pics", CC_STR("l10n/en/images.png"));
  addImage("game-pics", CC_STR("pics/sprites.png"));
  addImage("img-pics", CC_STR("pics/images.png"));

  addImage("gui.mmenus.menu.bg", CC_STR("pics/bg.png"));
  addImage("game.bg", CC_STR("pics/bg.png"));

  addEffect("game_end", CC_STR("sfx/MineExplosion.mp3"));
  addEffect("game_quit", CC_STR("sfx/Death.mp3"));
  addEffect("ball-paddle", CC_STR("sfx/ElevatorBeep.mp3"));
  addEffect("ball-brick", CC_STR("sfx/MineBeep.mp3"));

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
void Config::initLevels() {
  auto d= getLevel("1");
  auto j= j::json({
        {"PADDLE+SPEED", 150},
        {"BALL+SPEED", 200},
        {"COLS", 9},
        {"TOP", 6},
        {"TOP+ROW", 10},
        {"ROWS",  j::json::array_t {0,1,5,3,4} },
        {"CDS", j::json::object_t {
          {"red_candy.png", 10 },
          {"amber_candy.png", 15},
          {"white_candy.png", 8},
          {"green_candy.png", 20},
          {"yellow_candy.png", 15},
          {"blue_candy.png",  10},
          {"purple_plus_candy.png", 5},
          {"purple_minus_candy.png", 15 }
        }}
      });
  d->setObject(f::JsonObj::create(j), CFG);
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::handleResolution(const c::Size &rs) {
  auto gz= gameSize();
  //for default font, we use 48pt
  scale = 52/256 * rs.width / gz.width;
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::runOnce() {
  auto c= c::SpriteFrameCache::getInstance();
  auto fp= getAtlas("game-pics");
  c->addSpriteFramesWithFile( fp);
  CCLOG("loaded sprite-sheet: %s", fp.c_str());

  fp= getAtlas("img-pics");
  c->addSpriteFramesWithFile( fp);
  CCLOG("loaded sprite-sheet: %s", fp.c_str());

  fp= getAtlas("lang-pics");
  c->addSpriteFramesWithFile(fp);
  CCLOG("loaded sprite-sheet: %s", fp.c_str());
}


NS_END(breakout)


