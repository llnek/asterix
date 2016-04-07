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
NS_BEGIN(terra)

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

  _game_id= "4d6b93c4-05d7-42f1-95cc-98ce8adeac0a";
  _app_id= "terra";

  addCst("MISSILE+SPEED", CC_FLOAT(900.f));
  addCst("BOMB+SPEED", CC_FLOAT(200.f));
  addCst("SHIP+SPEED", CC_FLOAT(200.f));
  addCst("SHIP+ZX", CC_INT(3000));

  addCst("menuHeight", CC_INT(36));
  addCst("menuWidth", CC_INT(123));
  addCst("flareY", CC_INT(445));

  addColor("text", CC_STR("#ffffff"));
}

//////////////////////////////////////////////////////////////////////////////
//
float Config::scaleFont(float pt) {
  return pt/128.0 * _scale;
}

//////////////////////////////////////////////////////////////////////////
//
void Config::initAssets() {

  addAtlas("game-pics", CC_STR("pics/textureTransparentPack.plist"));
  addAtlas("cc-pics", CC_STR("pics/sprite_sheet.plist"));
  addAtlas("op-pics", CC_STR("pics/textureOpaquePack.plist"));
  addAtlas("explosions", CC_STR("pics/explosion.plist"));
  addAtlas("back-tiles", CC_STR("pics/b01.plist"));

  addImage("game-pics", CC_STR("pics/textureTransparentPack.png"));
  addImage("cc-pics", CC_STR("pics/sprite_sheet.png"));
  addImage("op-pics", CC_STR("pics/textureOpaquePack.png"));
  addImage("explosions", CC_STR("pics/explosion.png"));
  addImage("back-tiles", CC_STR("pics/b01.png"));

  addImage("flare", CC_STR("pics/flare.jpg"));
  addImage("game.bg", CC_STR("pics/bg.png"));
  addImage("gui.xxx.bg", CC_STR("pics/gui.png"));
  addImage("explosions", CC_STR("pics/explosion.png"));

  addEffect("shipDestroyEffect", CC_STR("sfx/shipDestroyEffect.mp3"));
  addEffect("bgMusic", CC_STR("sfx/bgMusic.mp3"));
  addEffect("btnEffect", CC_STR("sfx/buttonEffet.mp3"));
  addEffect("explodeEffect", CC_STR("sfx/explodeEffect.mp3"));
  addEffect("fireEffect", CC_STR("sfx/fireEffect.mp3"));
  addMusic("mainMusic", CC_STR("sfx/mainMainMusic.mp3"));

  addFont("title", CC_STR("fonts/en/ChunkyBlocks.fnt"));
  addFont("score", CC_STR("fonts/en/Hiruko.fnt"));
  addFont("btns", CC_STR("fonts/en/Hiruko.fnt"));
  addFont("dft", CC_STR("fonts/en/Quicksand.fnt"));
  addFont("text", CC_STR("fonts/en/Verdana.ttf"));
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
void Config::handleResolution(const CCT_SZ &rs) {
  auto gz= gameSize();
//  CC_DTOR()->setContentScaleFactor(0.5);
  //for default font, we use 48pt
//  _scale = 52.0f/256.0f * rs.width / gz.width;
}

//////////////////////////////////////////////////////////////////////////
//
void Config::runOnce() {

  cacheSprites("game-pics");
  cacheSprites("cc-pics");
  cacheSprites("op-pics");
  cacheSprites("explosions");
  cacheSprites("back-tiles");

  auto anim= cx::createAnimation(1);
  anim->addSpriteFrame(cx::getSpriteFrame("ship01.png"));
  anim->addSpriteFrame(cx::getSpriteFrame("ship02.png"));
  CC_ACAC()->addAnimation(anim, "ShipAni");
}



NS_END


