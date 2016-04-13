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
NS_BEGIN(hockey)


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
  _game_id= "6943f96b-c913-436a-b1c9-6bcb8d08fc5c";
  _app_id = "hockey";
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initAssets() {

  addAtlas("cc-pics", CC_STR("pics/sprite_sheet.plist"));
  addAtlas("game-pics", CC_STR("pics/images.plist"));

  addImage("cc-pics", CC_STR("pics/sprite_sheet.png"));
  addImage("game-pics", CC_STR("pics/images.png"));

  addImage("game.bg", CC_STR("pics/court.jpg"));

  addFont("title", CC_STR("fonts/en/SFCollegiate.fnt"));
  addFont("dft", CC_STR("fonts/en/SVBasicManual.fnt"));
  addFont("btns", CC_STR("fonts/en/Hiruko.fnt"));
  addFont("text", CC_STR("fonts/en/Verdana.ttf"));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::handleResolution(const CCT_SZ &fz) {
  auto gz= gameSize();
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::runOnce() {
  cacheSprites("game-pics");
  cacheSprites("cc-pics");
}

//////////////////////////////////////////////////////////////////////////////
//
float Config::scaleFont(float pt) {
  return pt/128.0 * _scale;
}

//////////////////////////////////////////////////////////////////////////////
//
c::Scene* Config::prelude() {
  return Splash::reify();
}


NS_END


