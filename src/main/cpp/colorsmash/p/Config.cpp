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
#include "n/C.h"

NS_BEGIN(colorsmash)

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
  auto j= j::json({
      // number of tiles used to trigger bonuses eg. Bonus if 50 tiles collected in one shot
      {"BONUS", j::json({50,40,30,20,10  }) }
  });
  d->setObject(f::JsonObj::create(j), CFG);
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initCsts() {

  _game_id="9ff45f09-570d-4563-a80d-106ba0912cd1";
  _app_id = "colorsmash";

  // offset so that game is not stuck to the bottom-left
  addCst("GAMEPLAY+OFFSET+X", CC_FLOAT(TILE_SIZE/2));
  addCst("GAMEPLAY+OFFSET+Y", CC_FLOAT(TILE_SIZE));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initAssets() {

  addAtlas("cc-pics", CC_STR("pics/sprite_sheet.plist"));
  addImage("cc-pics", CC_STR("pics/sprite_sheet.png"));

  addAtlas("game-pics", CC_STR("pics/images.plist"));
  addImage("game-pics", CC_STR("pics/images.png"));

  addEffect("button", CC_STR("sfx/button-click.wav"));
  addEffect("crash", CC_STR("sfx/crash.wav"));
  addMusic("background", CC_STR("sfx/music.mp3"));

  addFont("title", CC_STR("fonts/en/SFCollegiate.fnt"));
  addFont("btns", CC_STR("fonts/en/Hiruko.fnt"));
  addFont("dft", CC_STR("fonts/en/SVBasicManual.fnt"));
  addFont("text", CC_STR("fonts/en/Verdana.ttf"));
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


