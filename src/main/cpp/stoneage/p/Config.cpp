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
NS_BEGIN(stoneage)

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
  game_id= "6acae825-6061-48ac-92b2-cdcf055ef23b";
  app_id = "stoneage";

  addCst("TILE_SIZE", CC_INT(60));
  addCst("GRID_SIZE_X", CC_INT(8));
  addCst("GRID_SIZE_Y", CC_INT(11));
  addCst("GRID_SPACE", CC_INT(6));

  addCst("Z_GRID", CC_INT(1));
  addCst("Z_SWAP_1", CC_INT(2));
  addCst("Z_SWAP_2", CC_INT(3));
  addCst("Z_DIAMOND", CC_INT(4));

  addCst("DIAMOND_POINTS", CC_INT(100));
  addCst("POINTS", CC_INT(10));
  addCst("COMBO_POINTS", CC_INT(50));

}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initAssets() {

  addImage("intro.bg", CC_STR("pics/introbg.jpg"));

  addMusic("background", CC_STR("sfx/background.mp3"));
  addEffect("match", CC_STR("sfx/match.wav"));
  addEffect("diamond", CC_STR("sfx/diamond.wav"));
  addEffect("diamond2", CC_STR("sfx/diamond2.wav"));
  addEffect("wrong", CC_STR("sfx/wrong.wav"));

  addFont("OCR", CC_STR("fon/en/OCR.fnt"));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::handleResolution(const c::Size &rs) {
  auto dz= gameSize();
  sstr p;
  int w;
  if (rs.width > 320) {
    w=640;
    p="hd";
  } else {
    w=320;
    p="sd";
  }

  c::FileUtils::getInstance()->setSearchPaths(s_vec<sstr> {p});
  CC_DTOR()->setContentScaleFactor(w/dz.width);
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::runOnce() {
  auto c= c::SpriteFrameCache::getInstance();
  //auto fp= getAtlas("game-pics");
  //c->addSpriteFramesWithFile( fp);
  //CCLOG("loaded sprite-sheet: %s", fp.c_str());
}

//////////////////////////////////////////////////////////////////////////////
//
c::Scene* Config::prelude() {
  return Splash::reify();
}


NS_END


