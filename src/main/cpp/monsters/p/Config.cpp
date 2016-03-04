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

  game_id= "aff2dd9a-2fa4-487e-8eea-f2ce786a36e8";
  app_id = "monsters";

  addCst("SHIP+SPEED", CC_INT(200));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initAssets() {

  addAtlas("game-pics", CC_STR("pics/Sprites.plist"));
  addImage("game-pics", CC_STR("pics/Sprites.png"));

  addImage("game.bg", CC_STR("pics/background.png"));

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


  addFont("dft", CC_STR("fon/en/Courier.fnt"));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::handleResolution(const c::Size &rs) {
  auto gz= gameSize();
  if (rs.width == gz.width) {
    CC_DTOR()->setContentScaleFactor(1);
  }
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::runOnce() {
  auto c= c::SpriteFrameCache::getInstance();
  auto fp= getAtlas("game-pics");
  c->addSpriteFramesWithFile( fp);
  CCLOG("loaded sprite-sheet: %s", fp.c_str());
}

//////////////////////////////////////////////////////////////////////////////
//
c::Scene* Config::prelude() {
  return Splash::reify();
}


NS_END


