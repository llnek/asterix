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
NS_BEGIN(victorian)

  /*
     resources: (sd) 32pt, 512 png,  507x380 bg
     resources: (hd) 60pt, 1024 png,  1024x768 bg
     resources: (rd) 120pt, 2048 png,  2048x1536 bg
   *
   * */
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
  _game_id= "cd5dfd6a-6941-4377-ae1a-9d624afd1127";
  _app_id = "victorian";
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initAssets() {

  addAtlas("game-pics",CC_STR("pics/sprite_sheet.plist"));

  addImage("game-pics",CC_STR("pics/sprite_sheet.png"));
  addImage("game.bg",CC_STR("pics/bg.png"));

  addFont("text", CC_STR("fon/en/Times.ttf"));
  addFont("dft", CC_STR("fon/en/font.fnt"));

  addMusic("background3",CC_STR("sfx/background3.mp3"));
  addEffect("falling",CC_STR("sfx/falling.wav"));
  addEffect("click",CC_STR("sfx/button-click.wav"));
  addEffect("hitBuilding",CC_STR("sfx/hitBuilding.wav"));
  addEffect("jump",CC_STR("sfx/jump.wav"));
  addEffect("crashing",CC_STR("sfx/crashing.wav"));
  addEffect("start",CC_STR("sfx/start.wav"));
  addEffect("openUmbrella",CC_STR("sfx/openUmbrella.wav"));

}

//////////////////////////////////////////////////////////////////////////////
//
void Config::handleResolution(const CCT_SZ &rs) {
  auto dz= gameSize();
  auto p="sd";
  auto h=380.0;

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
  CC_FILER()->setSearchPaths(s_vec<sstr> {p});
}

//////////////////////////////////////////////////////////////////////////////
//
float Config::scaleFont(float f) {
    return f;
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::runOnce() {
  cacheSprites("game-pics");
}

//////////////////////////////////////////////////////////////////////////////
//
c::Scene* Config::prelude() {
  return Splash::reify();
}


NS_END


