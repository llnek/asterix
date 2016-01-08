// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2015, Ken Leung. All rights reserved.

#include "Config.h"
#include "Splash.h"

NS_BEGIN(asteroids)

//////////////////////////////////////////////////////////////////////////////
//
owner<Config*> Config::reify() {
  auto c= mc_new(Config);
  return c;
}

//////////////////////////////////////////////////////////////////////////////
//
c::Scene* Config::prelude() {
  return Splash::reify();
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initCsts() {
  game_id= "339a5c13-24b3-4069-9a0a-661820573fb3";
  app_id= "asteroids";

  addCst("P_LMS", CC_STR("live-missiles"));
  addCst("P_LLS",  CC_STR("live-lasers"));
  addCst("P_MS", CC_STR("missiles"));
  addCst("P_LS", CC_STR("lasers"));
  addCst("P_LAS", CC_STR("live-asteroids"));

  addCst("P_AS3", CC_INT(3));
  addCst("P_AS2", CC_INT(2));
  addCst("P_AS1", CC_INT(1));

  addCst("GRID_W", CC_INT(60));
  addCst("GRID_H", CC_INT(40));

}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initAssets() {

  addAtlas("lang-pics",  CC_STR("l10n/en/images.plist"));
  addAtlas("game-pics", CC_STR("pics/sprites.plist"));

  addImage("lang-pics",  CC_STR("l10n/en/images.png"));
  addImage("game-pics", CC_STR("pics/sprites.png"));
  addImage("gui.mmenus.menu.bg", CC_STR("pics/bg.png"));
  addImage("game.bg", CC_STR("pics/bg.png"));

  addEffect("game_end", CC_STR("sfx/MineExplosion.mp3"));
  addEffect("game_quit", CC_STR("sfx/Death.mp3"));

}

//////////////////////////////////////////////////////////////////////////////
//
void Config::addLevels() {
  auto d= getLevel("1");
  auto j= j::json({

      {"large", j::json({
          {"img", "rock_large.png"},
          {"num", 5},
          {"value", 25}
          }) },
      {"med", j::json({
          {"img", "rock_med.png"},
          {"num", 5},
          {"value", 50}
          }) },
      {"small", j::json({
          {"img", "rock_small.png"},
          {"num", 10},
          {"value", 100}
          }) }
  });
  d->setObject(f::JsonObj::create(j), CFG);
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::handleResolution(const c::Size &rs) {
  auto gz= gameSize();
  //for default font, we use 48pt
  scale = 52/256.0f * rs.width / gz.width;
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::runOnce() {
  auto c= c::SpriteFrameCache::getInstance();
  auto fp= getAtlas("game-pics");
  c->addSpriteFramesWithFile( fp);
  CCLOG("loaded sprite-sheet: %s", fp.c_str());
  fp= getAtlas("lang-pics");
  c->addSpriteFramesWithFile(fp);
  CCLOG("loaded sprite-sheet: %s", fp.c_str());
}


NS_END(asteroids)

