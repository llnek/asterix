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

NS_BEGIN(breakout)

BEGIN_NS_UNAMED
const s_arr<sstr,8> CANDIES {
  "red_candy.png", "amber_candy.png","white_candy.png","green_candy.png",
  "yellow_candy.png","blue_candy.png", "purple_plus_candy.png", "purple_minus_candy.png"
};
END_NS_UNAMED
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
  assert(pos >= 0 && pos < CANDIES.size());
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
  _game_id = "7d943e06-0849-4bf4-a16d-64a401f72a3e";
  _app_id= "breakout";

  addCst("GRID_W", CC_INT(40));
  addCst("GRID_H", CC_INT(60));

  addCst("PADDLE+OFF", CC_INT(4));
  addCst("LEFT+OFF", CC_INT(4));

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

  addEffect("game_end", CC_STR("sfx/end.mp3"));
  addEffect("game_quit", CC_STR("sfx/quit.mp3"));
  addEffect("ball-paddle", CC_STR("sfx/paddle.mp3"));
  addEffect("ball-brick", CC_STR("sfx/brick.mp3"));

  addEffect("click", CC_STR("sfx/button-click.wav"));
  addMusic("background", CC_STR("sfx/music.mp3"));

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
void Config::handleResolution(const CCT_SZ &rs) {
  auto gz= gameSize();
}

//////////////////////////////////////////////////////////////////////////////
//
float Config::scaleFont(float pt) {
  return pt/128.0 * _scale;
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::runOnce() {
  cacheSprites("game-pics");
  cacheSprites("cc-pics");
}


NS_END


