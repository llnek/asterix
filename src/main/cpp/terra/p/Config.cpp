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

#include "Config.h"

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////
//
owner<f::XConfig*> Config::reify() {
  auto c =  mc_new(Config);
  c->initAssets();
  c->initCsts();
  c->initLevels();
  return c;
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initCsts() {

  addCst("MISSILE_SPEED", CC_FLOAT(900.f));
  addCst("BOMB_SPEED", CC_FLOAT(200.f));
  addCst("SHIP_SPEED", CC_FLOAT(200.f));
  addCst("SHIP_ZX", CC_INT(3000));

  addCst("menuHeight", CC_INT(36));
  addCst("menuWidth", CC_INT(123));
  addCst("flareY", CC_INT(445));
}

//////////////////////////////////////////////////////////////////////////
//
void Config::initAssets() {

  addAtlas("game-pics", CC_STR("pics/textureTransparentPack"));
  addAtlas("lang-pics", CC_STR("l10n/{{lang}}/images"));
  addAtlas("op-pics", CC_STR("pics/textureOpaquePack"));
  addAtlas("explosions", CC_STR("pics/explosion"));
  addAtlas("back-tiles", CC_STR("pics/b01"));

  addImage("gui.mmenus.menu.bg", CC_STR("pics/bg.png"));
  addImage("flare", CC_STR("pics/flare.jpg"));
  addImage("game.bg", CC_STR("pics/bg.png"));

  addEffect("shipDestroyEffect", CC_STR("sfx/shipDestroyEffect.mp3"));
  addEffect("bgMusic", CC_STR("sfx/bgMusic.mp3"));
  addEffect("btnEffect", CC_STR("sfx/buttonEffet.mp3"));
  addEffect("explodeEffect", CC_STR("sfx/explodeEffect.mp3"));
  addEffect("fireEffect", CC_STR("sfx/fireEffect.mp3"));
  addEffect("mainMusic", CC_STR("sfx/mainMainMusic.mp3"));

  addFont("font.arial", CC_STR("fon/en/arial-14.fnt"));

}

//////////////////////////////////////////////////////////////////////////
//
void Config::initLevels() {
  auto d = getLevel("1");
  assert(d != nullptr);

  auto cfg = j::json({

      {"enemyMax" , 6},

      {"enemies", j::json::array_t {

          { {"style", "*"},
            {"time",  2},
            {"types", j::json::array_t { 0,1,2} }},

          { {"style", "*"},
            {"time",  5},
            {"types", j::json::array_t { 3,4,5} }}

      }}});

  d->setObject(f::JsonObj::create(cfg), CFG);
}

//////////////////////////////////////////////////////////////////////////
//
void Config::handleResolution(const c::Size& rs) {
  //for default font, we use 48pt
  scale = 52.0f/256.0f * rs.width /320.0f;
}

//////////////////////////////////////////////////////////////////////////
//
void Config::runOnce() {
  auto c= c::SpriteFrameCache::getInstance();
  auto fp= getAtlas("game-pics");

  c->addSpriteFramesWithFile(fp);
  CCLOG("loaded sprite-sheet: %s", fp.c_str());

  fp = getAtlas("lang-pics");
  c->addSpriteFramesWithFile(fp);
  CCLOG("loaded sprite-sheet: %s", fp.c_str());

  fp= getAtlas("op-pics");
  c->addSpriteFramesWithFile(fp);
  CCLOG("loaded sprite-sheet: %s", fp.c_str());

  fp = getAtlas("explosions");
  c->addSpriteFramesWithFile(fp);
  CCLOG("loaded sprite-sheet: %s", fp.c_str());

  fp = getAtlas("back-tiles");
  c->addSpriteFramesWithFile(fp);
  CCLOG("loaded sprite-sheet: %s", fp.c_str());
}







NS_END(terra)


