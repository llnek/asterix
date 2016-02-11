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

#include "Game.h"
#include "Config.h"
NS_BEGIN(victorian)

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
  game_id= "cd5dfd6a-6941-4377-ae1a-9d624afd1127";
  app_id = "victorian";
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initAssets() {
  addFont("dft", CC_STR("fon/en/font.fnt"));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::handleResolution(const c::Size &rs) {
  auto dz= gameSize();
  sstr p;
  float h;

  if (rs.height > 768) {
    p="rd";
    h= 1536;
  } else if (rs.height > 320) {
    p="hd";
    h=768;
  } else {
    p="sd";
    h=380;
  }

  CC_DTOR()->setContentScaleFactor(h/dz.height);
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
  return Game::reify();
}


NS_END


