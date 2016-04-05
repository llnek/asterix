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
NS_BEGIN(tttoe)

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
  auto j= j::json({
        {"HUMAN+THINK", 7},
        {"ROBOT+DELAY", 1000.0f}
      });
  d->setObject(f::JsonObj::create(j), CFG);
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initCsts() {
  addCst("engine", CC_STR("czlab.frigga.tttoe.core/TicTacToe"));
  addCst("pubdate", CC_STR("2014-06-03"));
  addCst("author", CC_STR("llnek"));
  addCst("minp", CC_INT(2));
  addCst("maxp", CC_INT(2));
  addCst("showFPS", CC_BOOL(false));

  addColor("text", CC_STR("#ffffff"));
  addColor("dft", CC_STR("#5e3178"));
  addColor("x", CC_STR("#f6b17f"));

  _game_id= "bd5f79bb-eb41-4ed5-bb44-2529dc27ed3c";
  _app_id = "tictactoe";
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initAssets() {

  addAtlas("lang-pics", CC_STR("l10n/en/images.plist"));
  addAtlas("game-pics", CC_STR("pics/images.plist"));

  addImage("gui.mmenu.menu.bg", CC_STR("pics/bg.png"));
  addImage("game.bg", CC_STR("pics/bg.png"));

  addImage("lang-pics", CC_STR("l10n/en/images.png"));
  addImage("game-pics", CC_STR("pics/images.png"));

  addEffect("game_end", CC_STR("sfx/end.mp3"));
  addEffect("x_pick", CC_STR("sfx/x_pick.mp3"));
  addEffect("o_pick", CC_STR("sfx/o_pick.mp3"));
  addEffect("game_quit", CC_STR("sfx/quit.mp3"));
  addEffect("button", CC_STR("sfx/click.wav"));
  addMusic("background", CC_STR("sfx/music.mp3"));

  addFont("dft", CC_STR("fonts/en/BelshawDonutRobot.fnt"));
  addFont("title", CC_STR("fonts/en/ChunkyBlocks.fnt"));
  addFont("timer", CC_STR("fonts/en/Hiruko.fnt"));
  addFont("text", CC_STR("fonts/en/Verdana.ttf"));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::handleResolution(const c::Size &rs) {
  auto gz= gameSize();
}

//////////////////////////////////////////////////////////////////////////////
//
float Config::scaleFont(float pt) {
  return pt / 128.0 * _scale;
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::scaleLabel(not_null<c::Label*> n, float sz) {
  n->setScale( scaleFont(sz));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::runOnce() {
  cacheSprites("game-pics");
  cacheSprites("lang-pics");
}

//////////////////////////////////////////////////////////////////////////////
//
c::Scene* Config::prelude() {
  return Splash::reify();
}



NS_END


