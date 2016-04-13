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

#include "core/CCSX.h"
#include "Splash.h"
#include "Config.h"

NS_ALIAS(cx, fusii::ccsx)
NS_BEGIN(invaders)

//////////////////////////////////////////////////////////////////////////////
//
c::Scene* Config::prelude() {
  return Splash::reify();
}

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
void Config::handleResolution(const CCT_SZ &rs) {
  auto z= this->gameSize();
}

//////////////////////////////////////////////////////////////////////////////
//
float Config::scaleFont(float pt) {
  return pt/128.0 * _scale;
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::runOnce() {
  auto anim= cx::createAnimation(0.1,false,0);
  for (auto i=0; i < 4; ++i) {
    anim->addSpriteFrame(
        cx::getSpriteFrame("boom_"+FTOS(i)+".png"));
  }
  CC_ACAC()->addAnimation(anim, "boom!");
  cacheSprites("game-pics");
  cacheSprites("cc-pics");
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initLevels() {
  auto d= getLevel("1");
  auto j= j::json({
        {"coolDownWindow",  800.0f},
        {"marching",  1000.0f},
        {"bombing",  2000.0f}
      });
  d->setObject(f::JsonObj::create(j), CFG);
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initAssets() {

  addAtlas("cc-pics", CC_STR("pics/sprite_sheet.plist"));
  addAtlas("game-pics", CC_STR("pics/images.plist"));

  addImage("cc-pics", CC_STR("pics/sprite_sheet.png"));
  addImage("game.bg", CC_STR("pics/bg.jpg"));
  addImage("game-pics", CC_STR("pics/images.png"));

  addEffect("game_end", CC_STR("sfx/end.mp3"));
  addEffect("game_quit", CC_STR("sfx/quit.mp3"));
  addEffect("ship-missile", CC_STR("sfx/missile.mp3"));
  addEffect("bugs-march", CC_STR("sfx/march.mp3"));
  addEffect("xxx-explode", CC_STR("sfx/explode.mp3"));
  addEffect("click", CC_STR("sfx/button-click.wav"));
  addMusic("background", CC_STR("sfx/music.mp3"));

  addFont("title", CC_STR("fonts/en/SFCollegiate.fnt"));
  addFont("btns", CC_STR("fonts/en/Hiruko.fnt"));
  addFont("dft", CC_STR("fonts/en/SVBasicManual.fnt"));
  addFont("text", CC_STR("fonts/en/Verdana.ttf"));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initCsts() {

  _game_id = "d39bf198-518a-4de7-88a0-5e28c88571b0";
  _app_id = "invaders";

  addColor("text", CC_STR("#ffffff"));
  addColor("dft", CC_STR("#5e3178"));

  addCst("showFPS", CC_BOOL(false));
  addCst("CELLS",  CC_INT(42));
  addCst("COLS",  CC_INT(6));
  addCst("ROWS",  CC_INT(7));
  addCst("LEFT", CC_INT(2));
  addCst("TOP", CC_INT(6));
  addCst("OFF_X", CC_INT(4));
  addCst("OFF_Y", CC_INT(2));

}


NS_END


