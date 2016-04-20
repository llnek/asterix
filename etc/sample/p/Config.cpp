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
NS_BEGIN(@@APPID@@)

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
  _game_id= "@@GAMEID@@";
  _app_id = "@@APPID@@";

}

//////////////////////////////////////////////////////////////////////////////
//
void Config::initAssets() {

  addAtlas("game-pics", CC_STR("pics/sprite_sheet.plist"));
  addImage("game-pics", CC_STR("pics/sprite_sheet.png"));

  addImage("game.bg", CC_STR("pics/background.png"));
  addImage("gui.bg", CC_STR("pics/bg.png"));

  addEffect("button", CC_STR("sfx/button-click.wav"));
  addEffect("crash", CC_STR("sfx/crash.wav"));
  addMusic("background", CC_STR("sfx/music.mp3"));

  addFont("dft", CC_STR("fonts/en/arial.ttf"));
}

//////////////////////////////////////////////////////////////////////////////
//
void Config::handleResolution(const CCT_SZ &rs) {
#if defined(BLAH)
  // retina iPad
  if (2048 == rs.width || 2048 == rs.height ) {
    resDirOrders.push_back("ipadhd");
    resDirOrders.push_back("ipad");
    resDirOrders.push_back("iphonehd5");
    resDirOrders.push_back("iphonehd");
    resDirOrders.push_back("iphone");
    glview->setDesignResolutionSize(1536, 2048, ResolutionPolicy::NO_BORDER);
  }
  else // non retina iPad
  if (1024 == rs.width || 1024 == rs.height ) {
    resDirOrders.push_back("ipad");
    resDirOrders.push_back("iphonehd5");
    resDirOrders.push_back("iphonehd");
    resDirOrders.push_back("iphone");
    glview->setDesignResolutionSize(768, 1024, ResolutionPolicy::NO_BORDER);
  }
  else // retina iPhone (5 and 5S)
  if (1136 == rs.width || 1136 == rs.height ) {
    resDirOrders.push_back("iphonehd5");
    resDirOrders.push_back("iphonehd");
    resDirOrders.push_back("iphone");
    glview->setDesignResolutionSize(640, 1136, ResolutionPolicy::NO_BORDER);
  }
  else // retina iPhone (4 and 4S)
  if ( 960 == rs.width || 960 == rs.height ) {
    resDirOrders.push_back("iphonehd");
    resDirOrders.push_back("iphone");
    glview->setDesignResolutionSize(640, 960, ResolutionPolicy::NO_BORDER);
  }
  else { // non retina iPhone and Android devices
    if (1080 < screenSize.width ) { // android devices that have a high resolution
      resDirOrders.push_back("iphonehd");
      resDirOrders.push_back("iphone");
      glview->setDesignResolutionSize(640, 960, ResolutionPolicy::NO_BORDER);
    } else { // non retina iPhone and Android devices with lower resolutions
      resDirOrders.push_back("iphone");
      glview->setDesignResolutionSize(320, 480, ResolutionPolicy::NO_BORDER);
    }
  }

  fileUtils->setSearchPaths(resDirOrders);
#endif
}

//////////////////////////////////////////////////////////////////////////////
//
float Config::scaleFont(float pt) {
  return pt/128.0 * _scale;
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


