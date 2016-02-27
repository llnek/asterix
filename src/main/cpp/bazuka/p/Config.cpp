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
NS_BEGIN(bazuka)

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

  game_id= "8ee5d415-c51e-4c4a-b577-9cfcbd4645ed";
  app_id = "bazuka";

  addCst("BG+SCROLL+SPEED", CC_FLOAT(3.0f));
  addCst("SHIP+SPEED", CC_INT(200));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initAssets() {

  addAtlas("game-pics", CC_STR("pics/sprite_sheet.plist"));
  addImage("game-pics", CC_STR("pics/sprite_sheet.png"));

  addImage("game.bg", CC_STR("pics/bg.png"));

  addEffect("rocketExplode", CC_STR("sfx/rocketExplode.wav"));
  addEffect("enemyKill", CC_STR("sfx/enemyKill.wav"));
  addEffect("fireRocket", CC_STR("sfx/fireRocket.wav"));
  addEffect("gunshot", CC_STR("sfx/gunshot.wav"));
  addEffect("playerKill", CC_STR("sfx/playerKill.wav"));
  addEffect("pop", CC_STR("sfx/pop.wav"));
  addMusic("bgMusic", CC_STR("sfx/bgMusic.wav"));

  addFont("dft","fon/en/PixelFont.fnt");
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::handleResolution(const c::Size &rs) {
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


