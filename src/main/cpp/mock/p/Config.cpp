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

#include "Game.h"
#include "Config.h"
NS_BEGIN(mock)

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
  _game_id= "c2371c3e-293a-4d27-880a-90e3a889e386";
  _app_id = "mock";
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initAssets() {

  addEffect("button", CC_STR("sfx/button-click.wav"));
  addEffect("crash", CC_STR("sfx/crash.wav"));
  addMusic("background", CC_STR("sfx/music.mp3"));

  addImage("blank", CC_STR("pics/blank.png"));

  addFont("text", CC_STR("fonts/en/arial.ttf"));
  addFont("dft", CC_STR("fonts/en/arial.ttf"));
}

//////////////////////////////////////////////////////////////////////////////
//
float Config::scaleFont(float pt) {
  return pt/128.0 * _scale;
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::handleResolution(const CCT_SZ &rs) {
  auto gz= gameSize();
  /*
  float h;
  sstr d="sd";

  if (rs.height > 768) {
    h=1536;
    d="rd";
  }
  else if (rs.height > 320) {
    h=768;
    d="hd";
  } else {
    h=380;
    d="sd";
  }

  CC_DTOR()->setContentScaleFactor(h/gz.height);
  CC_FILER()->setSearchPaths(s_vec<sstr>{d,"sfx"});
  */
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::runOnce() {
  //auto c= c::SpriteFrameCache::getInstance();
  //auto fp= getAtlas("game-pics");
  //c->addSpriteFramesWithFile( fp);
  //CCLOG("loaded sprite-sheet: %s", fp.c_str());
}

//////////////////////////////////////////////////////////////////////////////
//
c::Scene* Config::prelude() {
  return Game::reify(new GameCtx());
}


NS_END


