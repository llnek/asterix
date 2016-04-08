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
  _game_id= "f86694e1-5604-4d53-b684-eae696894546";
  _app_id= "spacecraze";

  addCst("star+speed+inc", CC_FLOAT(0.5f));
  addCst("star+speed", CC_FLOAT(10.0f));
  addCst("showFPS", CC_BOOL(false));
  addCst("MAX+STARS", CC_INT(15));

  addColor("text", CC_STR("#ffffff"));
  addColor("dft", CC_STR("#f6b17f"));

}

//////////////////////////////////////////////////////////////////////////////
//
float Config::scaleFont(float pt) {
  return pt/128.0 * _scale;
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initAssets() {

  addAtlas("explosions", CC_STR("misc/explosion.plist"));
  addAtlas("cc-pics", CC_STR("pics/sprite_sheet.plist"));
  addAtlas("game-pics", CC_STR("pics/spacetex.plist"));

  addImage("cc-pics", CC_STR("pics/sprite_sheet.png"));
  addImage("game-pics", CC_STR("pics/spacetex.png"));
  addImage("game.bg", CC_STR("pics/sfbg.jpg"));

  addEffect("level_complete", CC_STR("sfx/level_complete.wav"));
  addEffect("blast_brick", CC_STR("sfx/blast_brick.wav"));
  addEffect("blast_enemy", CC_STR("sfx/blast_enemy.wav"));
  addEffect("blast_player", CC_STR("sfx/blast_player.wav"));
  addEffect("game_over", CC_STR("sfx/game_over.wav"));
  addEffect("shoot_enemy", CC_STR("sfx/shoot_enemy.wav"));
  addEffect("shoot_player", CC_STR("sfx/shoot_player.wav"));

  addFont("title", CC_STR("fonts/en/ChunkyBlocks.fnt"));
  addFont("btns", CC_STR("fonts/en/Hiruko.fnt"));
  addFont("sftext", CC_STR("fonts/en/sftext.fnt"));
  addFont("score", CC_STR("fonts/en/sftext.fnt"));
  addFont("text", CC_STR("fonts/en/Verdana.ttf"));
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
void Config::handleResolution(const CCT_SZ &rs) {
  auto gz= gameSize();
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::runOnce() {
  cacheSprites("game-pics");
  cacheSprites("cc-pics");
}



NS_END


