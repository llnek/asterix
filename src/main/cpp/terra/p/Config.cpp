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
#include "Splash.h"
NS_BEGIN(terra)

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////
//
owner<Config*> Config::reify() {
  auto c =  mc_new(Config);
  c->initAssets();
  c->initCsts();
  c->initLevels();
  return c;
}

//////////////////////////////////////////////////////////////////////////////
//
c::Scene* Config::startWith() {
  return Splash::reify();
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

  addAtlas("game-pics", CC_STR("pics/textureTransparentPack.plist"));
  addAtlas("images-pics", CC_STR("pics/images.plist"));
  addAtlas("lang-pics", CC_STR("fon/en/images.plist"));
  addAtlas("op-pics", CC_STR("pics/textureOpaquePack.plist"));
  addAtlas("explosions", CC_STR("pics/explosion.plist"));
  addAtlas("back-tiles", CC_STR("pics/b01.plist"));

  addImage("game-pics", CC_STR("pics/textureTransparentPack.png"));
  addImage("images-pics", CC_STR("pics/images.png"));
  addImage("lang-pics", CC_STR("fon/en/images.png"));
  addImage("op-pics", CC_STR("pics/textureOpaquePack.png"));
  addImage("explosions", CC_STR("pics/explosion.png"));
  addImage("back-tiles", CC_STR("pics/b01.png"));


  addImage("gui.mmenus.menu.bg", CC_STR("pics/bg.png"));
  addImage("flare", CC_STR("pics/flare.jpg"));
  addImage("game.bg", CC_STR("pics/bg.png"));
  addImage("explosions", CC_STR("pics/explosion.png"));

  addEffect("shipDestroyEffect", CC_STR("sfx/shipDestroyEffect.mp3"));
  addEffect("bgMusic", CC_STR("sfx/bgMusic.mp3"));
  addEffect("btnEffect", CC_STR("sfx/buttonEffet.mp3"));
  addEffect("explodeEffect", CC_STR("sfx/explodeEffect.mp3"));
  addEffect("fireEffect", CC_STR("sfx/fireEffect.mp3"));
  addMusic("mainMusic", CC_STR("sfx/mainMainMusic.mp3"));

  addFont("font.SmallTypeWriting", CC_STR("fon/en/SmallTypeWriting.fnt"));
  addFont("font.AutoMission", CC_STR("fon/en/AutoMission.fnt"));
  addFont("font.Subito", CC_STR("fon/en/Subito.fnt"));
  addFont("font.CoffeeBuzzed", CC_STR("fon/en/CoffeeBuzzed.fnt"));

  addFont("font.TinyBoxBB", CC_STR("fon/en/TinyBoxBlackBitA8.fnt"));
  addFont("font.OogieBoogie", CC_STR("fon/en/OogieBoogie.fnt"));
  addFont("font.JellyBelly", CC_STR("fon/en/JellyBelly.fnt"));
  addFont("font.AgentOrange", CC_STR("fon/en/AgentOrange.fnt"));
  addFont("font.Hiruko", CC_STR("fon/en/Hiruko.fnt"));
  addFont("font.OCR", CC_STR("fon/en/OCR.fnt"));

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

  fp = getAtlas("images-pics");
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


