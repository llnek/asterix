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

#include "Splash.h"
#include "Config.h"
NS_BEGIN(monsters)

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

  _game_id= "aff2dd9a-2fa4-487e-8eea-f2ce786a36e8";
  _app_id = "monsters";

  addCst("SHIP+SPEED", CC_INT(200));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initAssets() {

  addAtlas("cc-pics", CC_STR("pics/sprite_sheet.plist"));
  addAtlas("game-pics", CC_STR("pics/Sprites.plist"));

  addImage("cc-pics", CC_STR("pics/sprite_sheet.png"));
  addImage("game-pics", CC_STR("pics/Sprites.png"));

  addImage("game.bg", CC_STR("pics/background.jpg"));

  addEffect("button", CC_STR("sfx/button-click.wav"));
  addEffect("attack", CC_STR("sfx/attack.wav"));
  addEffect("bigHit", CC_STR("sfx/bigHit.wav"));
  addEffect("boom", CC_STR("sfx/boom.wav"));
  addEffect("defend", CC_STR("sfx/defend.wav"));
  addEffect("lioncry", CC_STR("sfx/lioncry3.wav"));
  addEffect("lionlaugh", CC_STR("sfx/lionlaugh4.wav"));
  addEffect("mass", CC_STR("sfx/mass.wav"));
  addEffect("pew", CC_STR("sfx/pew.wav"));
  addEffect("smallHit", CC_STR("sfx/smallHit.wav"));
  addEffect("spawn", CC_STR("sfx/spawn.wav"));
  addMusic("background", CC_STR("sfx/Latin_Industries.mp3"));

  addFont("title", CC_STR("fonts/en/SFCollegiate.fnt"));
  addFont("btns", CC_STR("fonts/en/Hiruko.fnt"));
  addFont("dft", CC_STR("fonts/en/SVBasicManual.fnt"));
}

//////////////////////////////////////////////////////////////////////////////
//
float Config::scaleFont(float pt) {
  return pt/128.0 * _scale;
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::handleResolution(const CCT_SZ &rs) {
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
c::Scene* Config::prelude() {
  return Splash::reify();
}


NS_END


