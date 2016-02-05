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
#include "Game.h"
NS_BEGIN(hockey)

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
  game_id= "6943f96b-c913-436a-b1c9-6bcb8d08fc5c";
  app_id = "hockey";
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initAssets() {
  addImage("game.bg", CC_STR("pics/court.png"));
  addImage("mallet", CC_STR("pics/mallet.png"));
  addImage("puck", CC_STR("pics/puck.png"));
  addImage("logo", CC_STR("pics/logo.png"));
  addFont("arial", CC_STR("fon/en/arial.ttf"));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::handleResolution(const c::Size &fz) {
  auto gz= gameSize();
  sstr p;

  if (fz.width > gz.width) {
    CC_DTOR()->setContentScaleFactor(1);
    p="sd";
  } else {
    CC_DTOR()->setContentScaleFactor(2);
    p="hd";
  }

  c::FileUtils::getInstance()->setSearchPaths(s_vec<sstr> {p});
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::runOnce() {
}

//////////////////////////////////////////////////////////////////////////////
//
c::Scene* Config::prelude() {
  return Game::reify(mc_new(f::GCX));
}


NS_END


