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

#include "core/CCSX.h"
#include "Config.h"
#include "Splash.h"

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
c::Scene* Config::prelude() {
  return Splash::reify();
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initCsts() {

  _game_id = "fa0860f9-76dc-4135-8bc7-bd5af3147d55";
  _app_id = "pong";

  addColor("text", CC_STR("#32baf4"));
  addColor("dft", CC_STR("#edff90"));
  addColor("hud", CC_STR("#ffffff"));

  addCst("syncMillis", CC_FLOAT(3000));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initLevels() {
  auto d= getLevel("1");
  auto j= j::json({
        {"PADDLE+SPEED",  200.0f},
        {"BALL+SPEED",  150.0f},
        {"NUM+POINTS",  3}
      });
  d->setObject(f::JsonObj::create(j), CFG);
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

  addEffect("game_end", CC_STR("sfx/MineExplosion.mp3"));
  addEffect("x_hit", CC_STR("sfx/ElevatorBeep.mp3"));
  addEffect("o_hit", CC_STR("sfx/MineBeep.mp3"));
  addEffect("game_quit", CC_STR("sfx/Death.mp3"));
  addEffect("click", CC_STR("sfx/button-click.wav"));
  addMusic("background", CC_STR("sfx/music.mp3"));


  addFont("SmallTypeWriting", CC_STR("fon/en/SmallTypeWriting.fnt"));
  addFont("AutoMission", CC_STR("fon/en/AutoMission.fnt"));
  addFont("Subito", CC_STR("fon/en/Subito.fnt"));
  addFont("CoffeeBuzzed", CC_STR("fon/en/CoffeeBuzzed.fnt"));

  addFont("TinyBoxBB", CC_STR("fon/en/TinyBoxBlackBitA8.fnt"));
  addFont("OogieBoogie", CC_STR("fon/en/OogieBoogie.fnt"));
  addFont("JellyBelly", CC_STR("fon/en/JellyBelly.fnt"));
  addFont("AgentOrange", CC_STR("fon/en/AgentOrange.fnt"));
  addFont("Hiruko", CC_STR("fon/en/Hiruko.fnt"));
  addFont("OCR", CC_STR("fon/en/OCR.fnt"));

}

//////////////////////////////////////////////////////////////////////////////
//
void Config::handleResolution(const c::Size &rs) {
  auto z= gameSize();
  //for default font, we use 48pt
  scale = (52.0 / 256) * (rs.width / z.width);
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


NS_END


