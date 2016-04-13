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

#include "Config.h"
#include "Splash.h"

NS_BEGIN(tetris)

//////////////////////////////////////////////////////////////////////////////
//
owner<Config*> Config::reify() {
  auto c = mc_new(Config);
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

//////////////////////////////////////////////////////////////////////////
//
void Config::initCsts() {

  _game_id = "fb0fdd0b-1821-42d9-b6f7-26b11218b40d";
  _app_id = "tetris";

  addCst("THROTTLE+WAIT", CC_INT(120));
  addCst("BLOCK+COLORS", CC_INT(8));
  addCst("FIELD_W", CC_INT(12));
  addCst("BTN+SIZE", CC_INT(32));

  addCst("FENCE", CC_INT(0));
  addCst("TILE", CC_FLOAT(16.0f));

  addColor("text", CC_STR("#ffffff"));
  addColor("dft", CC_STR("#f6b17f"));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initAssets() {

  addAtlas("lang-pics", CC_STR("l10n/en/images.plist"));
  addAtlas("game-pics", CC_STR("pics/images.plist"));

  addImage("game.bg", CC_STR("pics/bg.png"));

  addImage("lang-pics", CC_STR("l10n/en/images.png"));
  addImage("game-pics", CC_STR("pics/images.png"));

  addEffect("button", CC_STR("sfx/button-click.wav"));
  addEffect("game_start", CC_STR("sfx/start.mp3"));
  addEffect("game_end", CC_STR("sfx/end.mp3"));
  addEffect("game_quit", CC_STR("sfx/quit.mp3"));
  addMusic("background", CC_STR("sfx/music.mp3"));

  addFont("title", CC_STR("fonts/en/ChunkyBlocks.fnt"));
  addFont("score", CC_STR("fonts/en/Hiruko.fnt"));
  addFont("dft", CC_STR("fonts/en/Quicksand.fnt"));

  addFont("text", CC_STR("fonts/en/Verdana.tff"));
}

//////////////////////////////////////////////////////////////////////////////
//
float Config::scaleFont(float pt) {
  return pt/128.0 * _scale;
}

//////////////////////////////////////////////////////////////////////////
//
void Config::initLevels() {
  auto d = getLevel("1");
  assert(d != nullptr);

  auto cfg = j::json({

      {"speed" , 1000.0f},
      {"nrate" , 1.0f},
      {"drate" , 6.0f}

      });

  d->setObject(f::JsonObj::create(cfg), CFG);
}


//////////////////////////////////////////////////////////////////////////////
//
void Config::handleResolution(const c::Size &rs) {
  auto gz= gameSize();
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::runOnce() {
  cacheSprites("game-pics");
  cacheSprites("lang-pics");
}



NS_END



