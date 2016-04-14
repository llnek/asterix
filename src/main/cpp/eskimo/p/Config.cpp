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

#include "Splash.h"
#include "Config.h"
NS_BEGIN(eskimo)

//////////////////////////////////////////////////////////////////////////////
//
owner<Config*> Config::reify() {
  auto c = mc_new(Config);
  c->initAssets();
  c->initLevels();
  c->initCsts();
  return c;
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initLevels() {
  auto d= getLevel("1");
  auto j= j::json({ });
  d->setObject(f::JsonObj::create(j), CFG);
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initCsts() {
  _game_id="9c1f0a53-1757-43df-bed1-50176a24eea2";
  _app_id = "eskimo";
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initAssets() {

  addAtlas("cc-pics", CC_STR("pics/sprite_sheet.plist"));
  addAtlas("game-pics", CC_STR("pics/images.plist"));

  addAtlas("levels",CC_STR("pics/levels.plist"));
  addAtlas("smoke",CC_STR("pics/smoke.plist"));
  addAtlas("snow",CC_STR("pics/snow.plist"));

  addImage("cc-pics", CC_STR("pics/sprite_sheet.png"));
  addImage("game-pics", CC_STR("pics/images.png"));
  addImage("game.bg", CC_STR("pics/bg.jpg"));

  addMusic("background", CC_STR("sfx/background.mp3"));
  addEffect("button", CC_STR("sfx/button.wav"));
  addEffect("cap", CC_STR("sfx/cap.wav"));
  addEffect("igloo", CC_STR("sfx/igloo.wav"));
  addEffect("oops", CC_STR("sfx/oops.wav"));
  addEffect("shape", CC_STR("sfx/shape.wav"));
  addEffect("switch", CC_STR("sfx/switch.wav"));

  addFont("font_msgs", CC_STR("fonts/en/font_messages.fnt"));
  addFont("font_levels", CC_STR("fonts/en/font_levels.fnt"));
  addFont("title", CC_STR("fonts/en/SFCollegiate.fnt"));
  addFont("btns", CC_STR("fonts/en/Hiruko.fnt"));
  addFont("text", CC_STR("fon/en/Verdana.ttf"));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::handleResolution(const CCT_SZ &rs) {
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

//////////////////////////////////////////////////////////////////////////////
//
c::Scene* Config::prelude() {
  return Splash::reify();
}


NS_END


