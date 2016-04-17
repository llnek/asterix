// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Kenneth Leung. All rights reserved.

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
  _game_id= "339a5c13-24b3-4069-9a0a-661820573fb3";
  _app_id= "asteroids";

  addCst("P_LMS", CC_STR("Live-Missiles"));
  addCst("P_LLS",  CC_STR("Live-Lasers"));
  addCst("P_MS", CC_STR("Missiles"));
  addCst("P_LS", CC_STR("Lasers"));
  addCst("P_LAS", CC_STR("Live-Asteroids"));

  addCst("THROTTLE+WAIT", CC_INT(300));

  addCst("GRID_W", CC_INT(60));
  addCst("GRID_H", CC_INT(40));

  addColor("text", CC_STR("#ffffff"));
  addColor("dft", CC_STR("#f6b17f"));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initAssets() {

  addAtlas("cc-pics", CC_STR("pics/sprite_sheet.plist"));
  addImage("cc-pics", CC_STR("pics/sprite_sheet.png"));

  addAtlas("game-pics", CC_STR("pics/images.plist"));
  addImage("game-pics", CC_STR("pics/images.png"));

  addImage("game.bg", CC_STR("pics/bg.jpg"));

  addEffect("game_quit", CC_STR("sfx/quit.mp3"));
  addEffect("game_end", CC_STR("sfx/end.mp3"));

  addFont("title", CC_STR("fonts/en/SFCollegiate.fnt"));
  addFont("btns", CC_STR("fonts/en/Hiruko.fnt"));
  addFont("dft", CC_STR("fonts/en/SVBasicManual.fnt"));
  addFont("text", CC_STR("fonts/en/Verdana.ttf"));

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
void Config::handleResolution(const CCT_SZ &rs) {
  auto gz= gameSize();
}

//////////////////////////////////////////////////////////////////////////////
//
float Config::scaleFont(float p) {
  return p/128.0 * _scale;
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::runOnce() {
  cacheSprites("game-pics");
  cacheSprites("cc-pics");
}


NS_END

