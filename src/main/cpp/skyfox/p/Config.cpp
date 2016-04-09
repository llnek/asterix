// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2016, Ken Leung. All rights reserved.

#include "Config.h"
#include "Splash.h"
NS_BEGIN(skyfox)

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
  _game_id= "f0c462b4-ba02-46e8-a991-508401172546";
  _app_id = "skyfox";
  addCst("UFO+SPEED", CC_INT(8));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initAssets() {

  addEffect("bombRelease", CC_STR("sfx/bombRelease.wav"));
  addEffect("fire_truck", CC_STR("sfx/fire_truck.wav"));
  addEffect("bombFail", CC_STR("sfx/bombFail.wav"));
  addEffect("boom", CC_STR("sfx/boom.wav"));
  addEffect("health", CC_STR("sfx/health.wav"));
  addEffect("pew", CC_STR("sfx/pew.wav"));
  addMusic("bg", CC_STR("sfx/background.mp3"));

  addAtlas("cc-pics", CC_STR("pics/sprite_sheet.plist"));
  addAtlas("game-pics", CC_STR("pics/dlist.plist"));

  addImage("cc-pics", CC_STR("pics/sprite_sheet.png"));
  addImage("game-pics", CC_STR("pics/dlist.png"));
  addImage("game.bg", CC_STR("pics/bg.jpg"));

  addFont("title", CC_STR("fonts/en/ChunkyBlocks.fnt"));
  addFont("btns", CC_STR("fonts/en/Hiruko.fnt"));
  addFont("text", CC_STR("fonts/en/Verdana.ttf"));
  addFont("dft", CC_STR("fonts/en/SFJuggernaut.fnt"));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::handleResolution(const CCT_SZ &fs) {
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

//////////////////////////////////////////////////////////////////////////////
//
c::Scene* Config::prelude() {
  return Splash::reify();
}


NS_END


