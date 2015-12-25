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

#include "core/CCSX.h"
#include "Config.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(pong)

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
void Config::initCsts() {

  addCst("BALL_SPEED", CC_FLOAT(150.0f)); // 25 increment
  addCst("PADDLE_SPEED", CC_FLOAT(200.0f));
  addCst("NUM_POINTS", CC_INT(4));

  addCst("GRID_W", CC_INT(40));
  addCst("GRID_H", CC_INT(60));

  addColor("default", CC_STR("#edff90"));
  addColor("text", CC_STR("#32baf4"));
  addColor("hud", CC_STR("#ffffff"));

  addCst("syncMillis", CC_INT(3000));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initAssets() {

  addAtlas("lang-pics", CC_STR("l10n/en/images.plist"));
  addAtlas("game-pics", CC_STR("pics/images.plist"));

  addImage("gui.mmenu.menu.bg", CC_STR("pics/bg.png"));
  addImage("game.bg", CC_STR("pics/bg.png"));

  addEffect("game_end", CC_STR("sfx/MineExplosion.mp3"));
  addEffect("x_hit", CC_STR("sfx/ElevatorBeep.mp3"));
  addEffect("o_hit", CC_STR("sfx/MineBeep.mp3"));
  addEffect("game_quit", CC_STR("sfx/Death.mp3"));

  addFont("font.SmallTypeWriting", CC_STR("fon/en/SmallTypeWriting.fnt"));
  addFont("font.AutoMission", CC_STR("fon/en/AutoMission.fnt"));
  addFont("font.Subito", CC_STR("fon/en/Subito.fnt"));
  addFont("font.CoffeeBuzzed", CC_STR("fon/en/CoffeeBuzzed.fnt"));

}

//////////////////////////////////////////////////////////////////////////////
//
void Config::handleResolution(const c::Size &rs) {
  //for default font, we use 48pt
  scale = 52.0f / 256 * rs.width /320.0f;
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::runOnce() {
  auto g= c::spriteFrameCache::getInstance();
  g->addSpriteFrames( "game-pics");

  g->addSpriteFrames( "lang-pics");
}


NS_END(pong)


