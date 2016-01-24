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

NS_BEGIN(asteroids)

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
c::Scene* Config::prelude() {
  return Splash::reify();
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initCsts() {
  game_id= "339a5c13-24b3-4069-9a0a-661820573fb3";
  app_id= "asteroids";

  addCst("P_LMS", CC_STR("Live-Missiles"));
  addCst("P_LLS",  CC_STR("Live-Lasers"));
  addCst("P_MS", CC_STR("Missiles"));
  addCst("P_LS", CC_STR("Lasers"));
  addCst("P_LAS", CC_STR("Live-Asteroids"));

  addCst("THROTTLE+WAIT", CC_INT(300));
  addCst("showFPS", CC_BOOL(false));

  addCst("GRID_W", CC_INT(60));
  addCst("GRID_H", CC_INT(40));

  addColor("text", CC_STR("#ffffff"));
  addColor("dft", CC_STR("#f6b17f"));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initAssets() {

  addAtlas("lang-pics",  CC_STR("l10n/en/images.plist"));
  addAtlas("game-pics", CC_STR("pics/sprites.plist"));
  addAtlas("img-pics", CC_STR("pics/images.plist"));

  addImage("lang-pics",  CC_STR("l10n/en/images.png"));
  addImage("game-pics", CC_STR("pics/sprites.png"));
  addImage("img-pics", CC_STR("pics/images.png"));

  addImage("gui.mmenus.menu.bg", CC_STR("pics/bg.png"));
  addImage("game.bg", CC_STR("pics/bg.png"));

  addEffect("game_end", CC_STR("sfx/MineExplosion.mp3"));
  addEffect("game_quit", CC_STR("sfx/Death.mp3"));

  addFont("SmallTypeWriting", CC_STR("fon/en/SmallTypeWriting.fnt"));
  addFont("AutoMission", CC_STR("fon/en/AutoMission.fnt"));
  addFont("Subito", CC_STR("fon/en/Subito.fnt"));
  addFont("CoffeeBuzzed", CC_STR("fon/en/CoffeeBuzzed.fnt"));

  addFont("TinyBoxBB", CC_STR("fon/en/TinyBoxBlackBitA8.fnt"));
  addFont("OogieBoogie", CC_STR("fon/en/OogieBoogie.fnt"));
  addFont("JellyBelly", CC_STR("fon/en/JellyBelly.fnt"));
  addFont("AgentOrange", CC_STR("fon/en/AgentOrange.fnt"));
  addFont("Hiruko", CC_STR("fon/en/Hiruko.fnt"));
  addFont("OCR", CC_STR("fon/en/OCR.fnt"));

}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initLevels() {
  auto d= getLevel("1");
  auto j= j::json({

      {"MISSILE+SPEED", 100},
      {"LASER+SPEED", 100},
      {"coolDownWnd", 800.0f},

      {"Astros1", j::json({
          {"img", "rock_large.png"},
          {"num", 5},
          {"speed", 10},
          {"value", 25}
          }) },
      {"Astros2", j::json({
          {"img", "rock_med.png"},
          {"num", 5},
          {"speed", 20},
          {"value", 50}
          }) },
      {"Astros3", j::json({
          {"img", "rock_small.png"},
          {"num", 10},
          {"speed", 30},
          {"value", 100}
          }) }

  });
  d->setObject(f::JsonObj::create(j), CFG);
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::handleResolution(const c::Size &rs) {
  auto gz= gameSize();
  //for default font, we use 48pt
  scale = 52/256.0f * rs.width / gz.width;
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


NS_END(asteroids)

