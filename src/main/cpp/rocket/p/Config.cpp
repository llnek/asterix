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
#include "Game.h"
NS_BEGIN(rocket)

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
  game_id= "47641fcb-a939-4fac-87e4-91e219c44cc8";
  app_id = "rocket";
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initAssets() {

  addAtlas("game-pics", CC_STR("pics/sprite_sheet.plist"));

  addImage("game-pics", CC_STR("pics/sprite_sheet.png"));
  addImage("game.bg",  CC_STR("pics/bg.png"));

  addEffect("rocket", CC_STR("sfx/rocket.wav"));
  addMusic("bg", CC_STR("sfx/background.mp3"));

  addFont("dft", CC_STR("fon/en/font.fnt"));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::handleResolution(const c::Size &rs) {
  auto dz= gameSize();
  sstr p;
  float w;

  if (rs.width > 768) {
    p= "rd";
    w=1536;
  } else if (rs.width > 320) {
    p="hd";
    w=768;
  } else {
    p="sd";
    w=380;
  }

  CC_DTOR()->setContentScaleFactor(w/dz.width);
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
    return Game::reify(mc_new(f::GCX));
}


NS_END


