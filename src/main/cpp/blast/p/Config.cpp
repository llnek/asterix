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

#include "Splash.h"
#include "Config.h"
NS_BEGIN(blast)

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
  _game_id= "@@GAMEID@@";
  _app_id = "blast";

  addCst("SHIP+SPEED", CC_INT(200));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initAssets() {

  addAtlas("game-pics", CC_STR("pics/sprite_sheet.plist"));
  addAtlas("iutexure", CC_STR("pics/iutexure.plist"));

  addImage("game-pics", CC_STR("pics/sprite_sheet.png"));
  addImage("iutexure", CC_STR("pics/iutexure.png"));

  addEffect("big_blast",CC_STR("sfx/big_blast.wav"));
  addEffect("blast_player", CC_STR("sfx/blast_player.wav"));
  addEffect("enemy_death",CC_STR("sfx/enemy_death.wav"));
  addEffect("game_over",CC_STR("sfx/game_over.wav"));
  addEffect("missile",CC_STR("sfx/missile.wav"));
  addEffect("shield",CC_STR("sfx/shield.wav"));
  addEffect("small_blast",CC_STR("sfx/small_blast.wav"));

  addEffect("button", CC_STR("sfx/button-click.wav"));
  addMusic("background", CC_STR("sfx/music.mp3"));

  addFont("dft", CC_STR("fon/en/infont.fnt"));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::handleResolution(const c::Size &rs) {

}

//////////////////////////////////////////////////////////////////////////////
//
void Config::runOnce() {
  auto fp= getAtlas("game-pics");
  CC_SCAC()->addSpriteFramesWithFile( fp);
  CCLOG("loaded sprite-sheet: %s", fp.c_str());

  fp= getAtlas("iutexure");
  CC_SCAC()->addSpriteFramesWithFile( fp);
  CCLOG("loaded sprite-sheet: %s", fp.c_str());
}

//////////////////////////////////////////////////////////////////////////////
//
c::Scene* Config::prelude() {
  return Splash::reify();
}


NS_END


