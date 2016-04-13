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

#include "Splash.h"
#include "Config.h"
#include "core/CCSX.h"

NS_ALIAS(cx,fusii::ccsx)
NS_BEGIN(flappy)

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
    _game_id= "516c557d-4402-4698-9fa5-72ad3ad34a3f";
  _app_id = "flappy";

  addCst("SHIP+SPEED", CC_INT(200));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initAssets() {

  addAtlas("game-pics", CC_STR("pics/sprite_sheet.plist"));
  addImage("game-pics", CC_STR("pics/sprite_sheet.png"));

  addAtlas("dhtex", CC_STR("pics/dhtex.plist"));
  addImage("dhtex", CC_STR("pics/dhtex.png"));

  addEffect("button", CC_STR("sfx/button-click.wav"));
  addEffect("crash", CC_STR("sfx/crash.mp3"));
  addEffect("flap", CC_STR("sfx/flap.mp3"));
  addMusic("background", CC_STR("sfx/music.mp3"));

  addFont("dft", CC_STR("fon/en/arial.ttf"));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::handleResolution(const c::Size &rs) {
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::runOnce() {
  auto c= c::SpriteFrameCache::getInstance();
  auto fp= getAtlas("game-pics");
  c->addSpriteFramesWithFile( fp);
  CCLOG("loaded sprite-sheet: %s", fp.c_str());

  fp= getAtlas("dhtex");
  c->addSpriteFramesWithFile( fp);
  CCLOG("loaded sprite-sheet: %s", fp.c_str());

  auto anim = c::Animation::create();
  anim->addSpriteFrame(cx::getSpriteFrame("dhch_1"));
  anim->addSpriteFrame(cx::getSpriteFrame("dhch_2"));
  anim->addSpriteFrame(cx::getSpriteFrame("dhch_3"));
  anim->addSpriteFrame(cx::getSpriteFrame("dhch_2"));
  anim->addSpriteFrame(cx::getSpriteFrame("dhch_1"));
  anim->setDelayPerUnit(0.1);
  c::AnimationCache::getInstance()->addAnimation(anim, "dragonFlying");

}

//////////////////////////////////////////////////////////////////////////////
//
c::Scene* Config::prelude() {
  return Splash::reify();
}


NS_END


