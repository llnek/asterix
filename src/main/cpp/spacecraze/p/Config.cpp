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

NS_BEGIN(spacecraze)

//////////////////////////////////////////////////////////////////////////////
//
owner<Config*> Config::reify() {
  auto c= mc_new(Config);
  c->initAssets();
  c->initCsts();
  c->initLevels();
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
  game_id= "f86694e1-5604-4d53-b684-eae696894546";
  app_id= "space craze";

  addCst("star+speed+inc", CC_FLOAT(0.5f));
  addCst("star+speed", CC_FLOAT(10.0f));
  addCst("showFPS", CC_BOOL(false));
  addCst("MAX+STARS", CC_INT(15));

  addColor("text", CC_STR("#ffffff"));
  addColor("dft", CC_STR("#f6b17f"));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initAssets() {

  //addAtlas("lang-pics",  CC_STR("l10n/en/images.plist"));
  //addAtlas("game-pics", CC_STR("pics/sprites.plist"));
  addAtlas("game-pics", CC_STR("pics/spacetex.plist"));

  //addImage("lang-pics",  CC_STR("l10n/en/images.png"));
  //addImage("game-pics", CC_STR("pics/sprites.png"));
  addImage("game-pics", CC_STR("pics/spacetex.png"));
  addImage("game.bg", CC_STR("pics/sfbg.png"));

  addEffect("level_complete", CC_STR("level_complete.wav"));
  addEffect("blast_brick", CC_STR("blast_brick.wav"));
  addEffect("blast_enemy", CC_STR("blast_enemy.wav"));
  addEffect("blast_player", CC_STR("blast_player.wav"));
  addEffect("game_over", CC_STR("game_over.wav"));
  addEffect("shoot_enemy", CC_STR("shoot_enemy.wav"));
  addEffect("shoot_player", CC_STR("shoot_player.wav"));

  //addFont("TinyBoxBB", CC_STR("fon/en/TinyBoxBlackBitA8.fnt"));

}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initLevels() {
  auto d= getLevel("1");
  auto j= j::json({
      { "BULLET+MOVE+DURATION", 1.5f },
      { "MAX+BULLETS", 25 },
      { "MAX+LEVELS", 5 }
  });
  d->setObject(f::JsonObj::create(j), CFG);
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::handleResolution(const c::Size &rs) {
  auto gz= gameSize();
  //for default font, we use 48pt
  scale = 48.0f * rs.width / gz.width;
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::runOnce() {
  auto c= c::SpriteFrameCache::getInstance();
  auto fp= getAtlas("game-pics");
  c->addSpriteFramesWithFile( fp);
  CCLOG("loaded sprite-sheet: %s", fp.c_str());

  //fp= getAtlas("lang-pics");
  //c->addSpriteFramesWithFile(fp);
  //CCLOG("loaded sprite-sheet: %s", fp.c_str());

}




NS_END


