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
  game_id= "f0c462b4-ba02-46e8-a991-508401172546";
  app_id = "skyfox";
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

  addAtlas("game-pics", CC_STR("pics/sprite_sheet.plist"));

  addImage("game-pics", CC_STR("pics/sprite_sheet.png"));
  addImage("game.bg", CC_STR("pics/bg.png"));

  addFont("dft", CC_STR("fon/en/font.fnt"));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::handleResolution(const c::Size &fs) {
  auto gz= gameSize();
  sstr p;
  if (fs.height > 768) {
    p="rd";
    CC_DTOR()->setContentScaleFactor(1536/gz.height);
  } else if (fs.height > 320) {
    p= "hd";
    CC_DTOR()->setContentScaleFactor(768/gz.height);
  } else {
    p="sd";
    CC_DTOR()->setContentScaleFactor(380/gz.height);
  }

    c::FileUtils::getInstance()->setSearchPaths(s_vec<sstr> {p});
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


